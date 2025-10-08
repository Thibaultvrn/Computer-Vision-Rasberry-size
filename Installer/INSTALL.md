# 🚀 Guide d'Installation

## Prérequis
- **Python 3.8 ou plus récent** installé sur votre système
- Connexion Internet pour télécharger les packages

---

## ✅ Installation en 3 étapes

### Étape 1 : Vérifier Python
Ouvrez PowerShell et vérifiez que Python est installé :

```powershell
python --version
```

Si Python n'est pas installé, téléchargez-le depuis [python.org](https://www.python.org/downloads/)

---

### Étape 2 : Installer les dépendances
Dans le dossier du projet, exécutez :

```powershell
pip install -r requirements.txt
```

Cela installera automatiquement :
- **opencv-python** (bibliothèque de vision par ordinateur)
- **numpy** (calculs mathématiques)
- **pyserial** (communication avec Arduino)

---

### Étape 3 : Tester l'installation
Vérifiez que tout fonctionne :

```powershell
python -c "import cv2; print('OpenCV version:', cv2.__version__)"
```

Vous devriez voir : `OpenCV version: 4.x.x`

---

## 🎯 Utilisation

### Analyser une image
```powershell
python raspberry_detector.py raspberry.jpg
```

### Avec échelle personnalisée (mm par pixel)
```powershell
python raspberry_detector.py raspberry.jpg --scale 0.3
```

### Mode sans affichage (pour scripts automatisés)
```powershell
python raspberry_detector.py raspberry.jpg --no-display
```

---

## 🔧 Calibration de l'échelle

Pour obtenir l'échelle correcte (mm/pixel) :

1. Placez un objet de référence de taille connue (ex: pièce de monnaie, règle)
2. Mesurez sa largeur en pixels dans l'image
3. Calculez : `scale = largeur_réelle_mm / largeur_pixels`

Exemple : Une pièce de 2€ (25.75 mm) mesure 50 pixels dans votre image
→ `scale = 25.75 / 50 = 0.515 mm/pixel`

---

## ❓ Problèmes courants

### Erreur "pip n'est pas reconnu"
Installez pip :
```powershell
python -m ensurepip --upgrade
```

### Erreur lors de l'installation d'opencv-python
Essayez la version headless (sans GUI) :
```powershell
pip install opencv-python-headless
```

### Python 3.8+ non disponible sur votre système
Téléchargez depuis : https://www.python.org/downloads/

---

## 📚 Ajustement des paramètres

Pour modifier les seuils de détection, éditez `raspberry_detector.py` :

```python
# Lignes 18-28 : Seuils HSV pour rouge/vert
RED_LOWER_1 = np.array([0, 70, 50])
RED_UPPER_1 = np.array([10, 255, 255])
# ...

# Ligne 31 : Seuil de maturité
RIPENESS_THRESHOLD = 0.5  # Ajustez entre 0 et 1
```

Pour calibrer interactivement les seuils HSV, vous pouvez utiliser des outils comme `hsv_color_picker.py` (je peux le créer si besoin).

---

## 🔗 Prochaines étapes

1. ✅ Installer Python et les dépendances
2. ✅ Tester avec une image de framboise
3. 📷 Calibrer l'échelle avec un objet de référence
4. 🎨 Ajuster les seuils HSV selon votre éclairage
5. 🔌 Intégrer avec Arduino (PySerial)

---

Pour toute question, consultez le [README.md](README.md) principal.
