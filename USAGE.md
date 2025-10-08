# 🚀 Guide d'utilisation rapide

## Compilation

```cmd
compile.bat
```

Cela compile `Algorith.C` et crée `Algorith.exe`.

## Exécution

```cmd
run.bat
```

Par défaut, teste avec `Rasberry.jpg` et échelle `0.023 mm/px`.

Ou avec paramètres personnalisés :
```cmd
run.bat votre_image.jpg 0.025
```

## Calibration de l'échelle

L'échelle convertit les pixels en millimètres. Pour la calculer :

1. Mesurez la largeur réelle d'une framboise (ex: 20 mm)
2. Exécutez avec échelle 1.0 : `run.bat test.jpg 1.0`
3. Notez la largeur en pixels (ex: 870 px)
4. Calculez : `échelle = 20 / 870 = 0.023 mm/px`

## Fichiers principaux

- `Algorith.C` - Code source du programme
- `compile.bat` - Script de compilation
- `run.bat` - Script d'exécution
- `Makefile` - Pour compilation Linux/WSL

## Voir aussi

- `README.md` - Documentation complète du projet
