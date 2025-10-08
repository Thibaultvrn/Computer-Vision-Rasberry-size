
/*
 * Algorith.C
 * Computer Vision en C (OpenCV C API)
 * Detecte la framboise la plus large et estime maturite (couleur) et taille.
 *
 * Compilation (Linux example):
 *   gcc Algorith.C -o algorith `pkg-config --cflags --libs opencv`
 * Under Windows you must link against your OpenCV installation libraries.
 *
 * Usage:
 *   ./algorith raspberry.jpg [scale_mm_per_pixel]
 *
 * Notes:
 * - This code uses the legacy OpenCV C API (IplImage, CvSeq, etc.).
 * - Assumptions made when details were missing:
 *   * Default scale: 0.5 mm per pixel (adjustable via command line).
 *   * Expected physical raspberry width range: 10 mm - 30 mm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

// Seuils HSV pour le rouge (OpenCV H range: 0-180)
// On prend deux intervalles (rouge bas et rouge haut) car le rouge entoure 0.
const CvScalar RED_LOW1  = {0, 70, 50, 0};   // H,S,V
const CvScalar RED_HIGH1 = {10, 255, 255, 0};
const CvScalar RED_LOW2  = {170, 70, 50, 0};
const CvScalar RED_HIGH2 = {180, 255, 255, 0};

// Seuil HSV pour le vert d'arriere-plan (à ajuster si nécessaire)
const CvScalar GREEN_LOW  = {35, 40, 40, 0};
const CvScalar GREEN_HIGH = {85, 255, 255, 0};

// Parametres attendus pour l'estimation physique
const double EXPECTED_MIN_MM = 10.0;
const double EXPECTED_MAX_MM = 30.0;

// Compte le nombre de pixels non-nuls dans une ROI d'une image 8u (1 canal)
static int count_nonzero_in_rect(IplImage* img, CvRect rect)
{
	int count = 0;
	int x0 = rect.x, y0 = rect.y, w = rect.width, h = rect.height;
	if (!img) return 0;
	// clamp rect
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	if (x0 + w > img->width) w = img->width - x0;
	if (y0 + h > img->height) h = img->height - y0;

	for (int y = y0; y < y0 + h; ++y) {
		unsigned char* row = (unsigned char*)(img->imageData + y * img->widthStep);
		for (int x = x0; x < x0 + w; ++x) {
			if (row[x] != 0) count++;
		}
	}
	return count;
}

int main(int argc, char** argv)
{
	const char* filename = "raspberry.jpg";
	double scale_mm_per_pixel = 0.5; // valeur par defaut

	if (argc >= 2) filename = argv[1];
	if (argc >= 3) scale_mm_per_pixel = atof(argv[2]);

	// Charger l'image BGR
	IplImage* src = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR);
	if (!src) {
		fprintf(stderr, "Erreur: impossible de charger l'image '%s'\n", filename);
		return 1;
	}

	// Convertir en HSV
	IplImage* hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cvCvtColor(src, hsv, CV_BGR2HSV);

	// Images masques 1-channel
	IplImage* red_mask1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* red_mask2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* red_mask  = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* green_mask = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* fruit_mask = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

	// Separer le rouge (deux plages)
	cvInRangeS(hsv, RED_LOW1, RED_HIGH1, red_mask1);
	cvInRangeS(hsv, RED_LOW2, RED_HIGH2, red_mask2);
	cvOr(red_mask1, red_mask2, red_mask, NULL);

	// Detecter le vert d'arriere-plan
	cvInRangeS(hsv, GREEN_LOW, GREEN_HIGH, green_mask);

	// Zone fruit = non-vert (on suppose que l'arriere-plan est vert)
	cvNot(green_mask, fruit_mask);

	// Nettoyage rapide: erosion + dilatation pour reduire bruit
	CvSize ksize = cvSize(3,3);
	cvSmooth(red_mask, red_mask, CV_MEDIAN, 3, 0, 0, 0);
	cvSmooth(fruit_mask, fruit_mask, CV_MEDIAN, 3, 0, 0, 0);

	// Trouver contours sur le masque rouge
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL;
	IplImage* red_mask_copy = cvCloneImage(red_mask); // cvFindContours modifie l'image
	int nContours = cvFindContours(red_mask_copy, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	if (!contours) {
		printf("No red contours found.\n");
	}

	// Iterer pour trouver la plus grande framboise (par largeur)
	CvSeq* c = contours;
	double best_width = 0.0;
	CvRect best_rect = cvRect(0,0,0,0);
	double best_color_ratio = 0.0;

	// Nous allons egalement garder la plus grande largeur en pixels (pour info)
	for (CvSeq* cnt = contours; cnt != NULL; cnt = cnt->h_next) {
		CvRect rect = cvBoundingRect(cnt, 0);
		double width_px = rect.width;

		// Calculer ratio: pixels rouges / pixels de la zone fruit dans la bounding box
		int red_pixels = count_nonzero_in_rect(red_mask, rect);
		int fruit_pixels = count_nonzero_in_rect(fruit_mask, rect);

		double ratio = 0.0;
		if (fruit_pixels > 0) ratio = (double)red_pixels / (double)fruit_pixels;

		// On selectionne la framboise avec la largeur maximale
		if (width_px > best_width) {
			best_width = width_px;
			best_rect = rect;
			best_color_ratio = ratio;
		}
	}

	if (best_width <= 0.0) {
		printf("Raspberry detected: NONE\n");
		// afficher images pour debug
		cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
		cvShowImage("Source", src);
		cvWaitKey(0);
		// cleanup
		cvReleaseImage(&src); cvReleaseImage(&hsv); cvReleaseImage(&red_mask1);
		cvReleaseImage(&red_mask2); cvReleaseImage(&red_mask); cvReleaseImage(&green_mask);
		cvReleaseImage(&fruit_mask); cvReleaseImage(&red_mask_copy); cvReleaseMemStorage(&storage);
		return 0;
	}

	// Decide ripe ou unripe selon le ratio
	const double RIPENESS_THRESHOLD = 0.5; // spec example
	int is_ripe = (best_color_ratio >= RIPENESS_THRESHOLD) ? 1 : 0;

	// Estimation largeur en mm
	double width_mm = best_width * scale_mm_per_pixel;

	// Confiance couleur: on normalise le ratio (on suppose ratio 0..1)
	double conf_color = best_color_ratio;
	if (conf_color < 0.0) conf_color = 0.0;
	if (conf_color > 1.0) conf_color = 1.0;

	// Confiance taille: basee sur l'intervalle attendu [EXPECTED_MIN_MM, EXPECTED_MAX_MM]
	double conf_size = (width_mm - EXPECTED_MIN_MM) / (EXPECTED_MAX_MM - EXPECTED_MIN_MM);
	if (conf_size < 0.0) conf_size = 0.0;
	if (conf_size > 1.0) conf_size = 1.0;

	// Affichage console
	printf("Raspberry detected: %s\n", is_ripe ? "RIPE" : "UNRIPE");
	printf("Estimated width: %.2f mm (%.1f px)\n", width_mm, best_width);
	printf("Confidence (color): %.2f\n", conf_color);
	printf("Confidence (size): %.2f\n", conf_size);

	// Dessiner contour et bounding box sur l'image source
	// dessiner tous les contours en jaune
	cvDrawContours(src, contours, CV_RGB(255,255,0), CV_RGB(255,255,0), 1, 2, 8, cvPoint(0,0));

	// rectangle de la meilleure framboise en rouge
	CvPoint pt1 = cvPoint(best_rect.x, best_rect.y);
	CvPoint pt2 = cvPoint(best_rect.x + best_rect.width, best_rect.y + best_rect.height);
	cvRectangle(src, pt1, pt2, CV_RGB(255,0,0), 2, 8, 0);

	// Ecrire textes
	char label[128];
	sprintf(label, "%s (%.2f mm)", is_ripe ? "RIPE" : "UNRIPE", width_mm);
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6, 0, 1, 8);
	cvPutText(src, label, cvPoint(best_rect.x, best_rect.y - 10), &font, CV_RGB(255,0,0));

	// Afficher resultats
	cvNamedWindow("Detected", CV_WINDOW_AUTOSIZE);
	cvShowImage("Detected", src);
	cvWaitKey(0);

	// Nettoyage
	cvReleaseImage(&src); cvReleaseImage(&hsv); cvReleaseImage(&red_mask1);
	cvReleaseImage(&red_mask2); cvReleaseImage(&red_mask); cvReleaseImage(&green_mask);
	cvReleaseImage(&fruit_mask); cvReleaseImage(&red_mask_copy);
	cvReleaseMemStorage(&storage);

	return 0;
}
