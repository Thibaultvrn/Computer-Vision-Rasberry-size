#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
raspberry_detector.py
Computer Vision pour détection de maturité et taille de framboises

Utilisation:
    python raspberry_detector.py <image_path> [--scale SCALE]

Exemple:
    python raspberry_detector.py raspberry.jpg --scale 0.5
"""

import cv2
import numpy as np
import argparse
import sys


# ========== PARAMÈTRES CONFIGURABLES ==========

# Seuils HSV pour le rouge (OpenCV H: 0-180, S: 0-255, V: 0-255)
# Le rouge est autour de H=0 et H=180, donc on prend deux intervalles
RED_LOWER_1 = np.array([0, 70, 50])
RED_UPPER_1 = np.array([10, 255, 255])
RED_LOWER_2 = np.array([170, 70, 50])
RED_UPPER_2 = np.array([180, 255, 255])

# Seuils HSV pour le vert (arrière-plan)
GREEN_LOWER = np.array([35, 40, 40])
GREEN_UPPER = np.array([85, 255, 255])

# Seuil de maturité (ratio de pixels rouges)
RIPENESS_THRESHOLD = 0.5

# Taille physique attendue (en mm)
EXPECTED_MIN_MM = 10.0
EXPECTED_MAX_MM = 30.0

# Échelle par défaut (mm par pixel)
DEFAULT_SCALE_MM_PER_PX = 0.5


# ========== FONCTIONS PRINCIPALES ==========

def detect_raspberries(image_path, scale_mm_per_px=DEFAULT_SCALE_MM_PER_PX, show_image=True):
    """
    Détecte la plus grande framboise rouge dans une image.
    
    Args:
        image_path: Chemin vers l'image à analyser
        scale_mm_per_px: Échelle de conversion pixels -> mm
        show_image: Si True, affiche l'image avec détections
        
    Returns:
        dict: Résultats de la détection (ripe, width_mm, confidences, etc.)
    """
    
    # 1. CHARGER L'IMAGE
    src = cv2.imread(image_path)
    if src is None:
        print(f"❌ Erreur: impossible de charger l'image '{image_path}'")
        return None
    
    print(f"✓ Image chargée: {src.shape[1]}x{src.shape[0]} pixels")
    
    # 2. CONVERTIR EN HSV
    hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
    
    # 3. CRÉER LES MASQUES
    # Masque rouge (deux intervalles combinés)
    red_mask_1 = cv2.inRange(hsv, RED_LOWER_1, RED_UPPER_1)
    red_mask_2 = cv2.inRange(hsv, RED_LOWER_2, RED_UPPER_2)
    red_mask = cv2.bitwise_or(red_mask_1, red_mask_2)
    
    # Masque vert (arrière-plan)
    green_mask = cv2.inRange(hsv, GREEN_LOWER, GREEN_UPPER)
    
    # Zone fruit = NON vert (on suppose que tout ce qui n'est pas vert est potentiellement fruit)
    fruit_mask = cv2.bitwise_not(green_mask)
    
    # 4. NETTOYAGE DES MASQUES (réduction du bruit)
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    red_mask = cv2.morphologyEx(red_mask, cv2.MORPH_CLOSE, kernel)
    red_mask = cv2.morphologyEx(red_mask, cv2.MORPH_OPEN, kernel)
    
    # 5. DÉTECTION DES CONTOURS ROUGES
    contours, _ = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    if len(contours) == 0:
        print("❌ Aucun contour rouge détecté")
        if show_image:
            cv2.imshow("Image source", src)
            cv2.imshow("Masque rouge", red_mask)
            cv2.waitKey(0)
            cv2.destroyAllWindows()
        return None
    
    print(f"✓ {len(contours)} contour(s) rouge(s) détecté(s)")
    
    # 6. TROUVER LA FRAMBOISE LA PLUS LARGE
    best_width = 0
    best_rect = None
    best_contour = None
    best_color_ratio = 0.0
    
    for contour in contours:
        # Calculer la bounding box
        x, y, w, h = cv2.boundingRect(contour)
        
        # Extraire la ROI (Region of Interest)
        roi_red = red_mask[y:y+h, x:x+w]
        roi_fruit = fruit_mask[y:y+h, x:x+w]
        
        # Compter les pixels
        red_pixels = cv2.countNonZero(roi_red)
        fruit_pixels = cv2.countNonZero(roi_fruit)
        
        # Calculer le ratio rouge / fruit
        color_ratio = red_pixels / fruit_pixels if fruit_pixels > 0 else 0.0
        
        # Sélectionner la plus large
        if w > best_width:
            best_width = w
            best_rect = (x, y, w, h)
            best_contour = contour
            best_color_ratio = color_ratio
    
    if best_rect is None:
        print("❌ Aucune framboise valide détectée")
        return None
    
    # 7. CALCUL DES RÉSULTATS
    x, y, w, h = best_rect
    
    # Maturité basée sur le ratio de rouge
    is_ripe = best_color_ratio >= RIPENESS_THRESHOLD
    
    # Largeur en mm
    width_mm = w * scale_mm_per_px
    
    # Confiance couleur (ratio normalisé entre 0 et 1)
    conf_color = min(1.0, max(0.0, best_color_ratio))
    
    # Confiance taille (basée sur la plage attendue)
    if width_mm < EXPECTED_MIN_MM:
        conf_size = width_mm / EXPECTED_MIN_MM
    elif width_mm > EXPECTED_MAX_MM:
        conf_size = 1.0 - min(1.0, (width_mm - EXPECTED_MAX_MM) / EXPECTED_MAX_MM)
    else:
        conf_size = (width_mm - EXPECTED_MIN_MM) / (EXPECTED_MAX_MM - EXPECTED_MIN_MM)
    conf_size = min(1.0, max(0.0, conf_size))
    
    # 8. AFFICHAGE CONSOLE
    print("\n" + "="*50)
    print(f"🍓 Raspberry detected: {'RIPE ✓' if is_ripe else 'UNRIPE ✗'}")
    print(f"📏 Estimated width: {width_mm:.2f} mm ({w} px)")
    print(f"🎨 Confidence (color): {conf_color:.2f}")
    print(f"📐 Confidence (size): {conf_size:.2f}")
    print("="*50 + "\n")
    
    # 9. VISUALISATION
    if show_image:
        # Dessiner tous les contours en jaune
        cv2.drawContours(src, contours, -1, (0, 255, 255), 2)
        
        # Dessiner la bounding box de la meilleure en rouge
        cv2.rectangle(src, (x, y), (x+w, y+h), (0, 0, 255), 3)
        
        # Ajouter le texte
        label = f"{'RIPE' if is_ripe else 'UNRIPE'} ({width_mm:.1f}mm)"
        cv2.putText(src, label, (x, y-10), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        
        # Afficher les images
        cv2.imshow("Detection Result", src)
        cv2.imshow("Red Mask", red_mask)
        cv2.imshow("Fruit Mask", fruit_mask)
        
        print("Appuyez sur une touche pour fermer les fenêtres...")
        cv2.waitKey(0)
        cv2.destroyAllWindows()
    
    # 10. RETOURNER LES RÉSULTATS
    return {
        'is_ripe': is_ripe,
        'width_px': w,
        'width_mm': width_mm,
        'confidence_color': conf_color,
        'confidence_size': conf_size,
        'bounding_box': (x, y, w, h),
        'color_ratio': best_color_ratio
    }


# ========== FONCTION PRINCIPALE ==========

def main():
    parser = argparse.ArgumentParser(
        description='Détecte la maturité et la taille de framboises dans une image'
    )
    parser.add_argument('image', help='Chemin vers l\'image à analyser')
    parser.add_argument('--scale', type=float, default=DEFAULT_SCALE_MM_PER_PX,
                        help=f'Échelle en mm/pixel (défaut: {DEFAULT_SCALE_MM_PER_PX})')
    parser.add_argument('--no-display', action='store_true',
                        help='Ne pas afficher les images (mode batch)')
    
    args = parser.parse_args()
    
    # Lancer la détection
    result = detect_raspberries(
        args.image, 
        scale_mm_per_px=args.scale,
        show_image=not args.no_display
    )
    
    if result is None:
        sys.exit(1)
    
    sys.exit(0)


if __name__ == '__main__':
    main()
