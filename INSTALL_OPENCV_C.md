# 🔧 Installation OpenCV pour C/C++ sous Windows

## 📋 Prérequis

1. **Visual Studio 2019 ou 2022** (Community Edition gratuite)
   - Télécharger : https://visualstudio.microsoft.com/fr/downloads/
   - Lors de l'installation, cocher : "Développement Desktop en C++"

2. **CMake** (pour compiler OpenCV)
   - Télécharger : https://cmake.org/download/
   - Version 3.20 ou supérieure

---

## 🚀 Méthode 1 : Installation via binaires pré-compilés (RAPIDE - 15 min)

### Étape 1 : Télécharger OpenCV

1. Aller sur : https://opencv.org/releases/
2. Télécharger **OpenCV 4.8.1 for Windows** (ou version plus récente)
3. Lancer le fichier `.exe` → Il va extraire dans `C:\opencv` (ou choisir un autre dossier)

### Étape 2 : Configurer les variables d'environnement

1. Ouvrir "Variables d'environnement système" (chercher dans Windows)
2. Dans "Variables système", trouver `Path` → Cliquer "Modifier"
3. Ajouter :
   ```
   C:\opencv\build\x64\vc15\bin
   C:\opencv\build\x64\vc16\bin
   ```
   (Adapter selon votre version Visual Studio : vc15=VS2017, vc16=VS2019, vc17=VS2022)

4. Créer une nouvelle variable système :
   - Nom : `OPENCV_DIR`
   - Valeur : `C:\opencv\build`

### Étape 3 : Tester l'installation

Ouvrir un **nouveau** PowerShell et taper :
```powershell
Test-Path $env:OPENCV_DIR
# Doit afficher : True
```

---

## 🚀 Méthode 2 : Compilation depuis les sources (LONG - 2h)

### Étape 1 : Télécharger les sources

```powershell
cd C:\
git clone https://github.com/opencv/opencv.git
cd opencv
git checkout 4.8.1
```

### Étape 2 : Compiler avec CMake

```powershell
mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 `
  -D CMAKE_BUILD_TYPE=Release `
  -D CMAKE_INSTALL_PREFIX=C:\opencv_build `
  -D BUILD_EXAMPLES=OFF `
  -D BUILD_TESTS=OFF `
  -D BUILD_PERF_TESTS=OFF ..

cmake --build . --config Release --target INSTALL
```

**⏱️ Cette étape prend 1-2 heures selon votre PC !**

### Étape 3 : Configurer les variables (même que Méthode 1)

---

## 🛠️ Compilation de Algorith.C

### Avec Visual Studio (GUI)

1. Ouvrir Visual Studio
2. Créer un nouveau projet : "Application Console C++"
3. Ajouter `Algorith.C` au projet
4. Configuration du projet :
   - **Propriétés du projet → C/C++ → Général → Autres répertoires Include**
     ```
     C:\opencv\build\include
     ```
   
   - **Propriétés du projet → Éditeur de liens → Général → Répertoires de bibliothèques supplémentaires**
     ```
     C:\opencv\build\x64\vc16\lib
     ```
   
   - **Propriétés du projet → Éditeur de liens → Entrée → Dépendances supplémentaires**
     ```
     opencv_world481.lib
     ```
     (Adapter `481` selon votre version : 4.8.1 → 481, 4.9.0 → 490, etc.)

5. Compiler : **Build → Build Solution** (F7)

### Avec ligne de commande (cl.exe)

Ouvrir "Developer Command Prompt for VS 2022" :

```cmd
cd C:\Users\thiba\Computer-Vision-Rasberry-size

cl /I"C:\opencv\build\include" Algorith.C ^
   /link /LIBPATH:"C:\opencv\build\x64\vc16\lib" ^
   opencv_world481.lib

algorith.exe test_image.jpg 0.5
```

---

## 🐧 Alternative : WSL (Windows Subsystem for Linux)

Si vous préférez Linux sous Windows (BEAUCOUP plus simple) :

### Installation

```powershell
# Dans PowerShell en administrateur
wsl --install
```

Redémarrer le PC, puis dans WSL (Ubuntu) :

```bash
# Installer OpenCV
sudo apt update
sudo apt install libopencv-dev

# Compiler
cd /mnt/c/Users/thiba/Computer-Vision-Rasberry-size
gcc Algorith.C -o algorith `pkg-config --cflags --libs opencv4`

# Exécuter
./algorith test_image.jpg 0.5
```

**✅ Cette méthode est recommandée si vous n'avez jamais compilé avec OpenCV sous Windows !**

---

## ⚠️ Problèmes courants

### Erreur : "cannot open source file cv.h"
→ Les chemins Include ne sont pas configurés. Vérifier les variables d'environnement.

### Erreur : "opencv_world481.lib introuvable"
→ Adapter le numéro de version (481, 490, etc.) selon votre installation.

### Erreur : "VCRUNTIME140.dll manquant"
→ Installer Visual C++ Redistributable : https://aka.ms/vs/17/release/vc_redist.x64.exe

### Le programme se lance mais crash immédiatement
→ Vérifier que les DLL OpenCV sont dans le PATH (voir Étape 2 de Méthode 1)

---

## 📚 Ressources

- Documentation OpenCV C : https://docs.opencv.org/4.x/d1/dfb/intro.html
- Tutoriel installation Windows : https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html
- Forum OpenCV : https://forum.opencv.org/

---

## ✅ Vérification finale

Une fois OpenCV installé et compilé, vous devriez pouvoir :

```cmd
algorith.exe raspberry.jpg 0.5
```

Et voir :
```
Raspberry detected: RIPE / UNRIPE
Estimated width: XX mm
Confidence (color): X.XX
Confidence (size): X.XX
```

Bon courage ! 🚀
