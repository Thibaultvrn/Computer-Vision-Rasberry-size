# 🍓 Computer Vision for Raspberry Ripeness & Size Detection
## ME-320 – Product & Engineering Design (EPFL)

### 🎯 Project Overview
This project is part of the **ME-320 Product and Engineering Design** course at EPFL.  
Our objective is to develop a **robotic gripper** capable of harvesting **soft fruits (raspberries, strawberries, blackberries)** without damaging them.

As part of this project, this program focuses on the **Computer Vision module**, whose goal is to:
- Detect whether a raspberry is **ripe or unripe** based on its **color**.
- Estimate the **size (diameter)** of the fruit in millimeters.
- Communicate this information to the gripper control system (Arduino) for decision-making.

---

### 🧠 Context
Harvesting soft fruits is a delicate process: too much force damages the fruit, and unripe fruits must be left on the plant.  
Our Computer Vision system assists the robotic gripper by:
1. Identifying **ripe fruits** (red or dark color, higher sugar content).  
2. Measuring their **approximate size** (between **10 mm and 30 mm** diameter, as specified in the course requirements).  
3. Sending the detection signal to the **Arduino control unit** via serial communication.

---

### 🧩 System Architecture
The system is divided into two main parts:

| Component | Role | Language |
|------------|------|-----------|
| **Arduino (C++)** | Controls actuators and sensors of the gripper | C / C++ |
| **Computer Vision (C)** | Detects fruit ripeness and size using OpenCV C API | C |

Communication between modules via **serial interface**.

---

### ⚙️ Technologies Used
- **C Programming Language**
- **OpenCV C API** – Image capture & color segmentation
- **Arduino C/C++** – For actuator control and serial communication

---

### 🧪 Detection Principles

1. **Color Analysis**  
   - Convert image to **HSV color space**.  
   - Define color ranges for:
     - Ripe: deep red / dark tones
     - Unripe: light red / white  
   - Apply masks and count red pixels.

2. **Size Estimation**  
   - Detect contours in the image.  
   - Approximate the diameter of the detected fruit region in pixels.  
   - Convert pixels → millimeters using a reference object or camera calibration.  
   - Expected diameter range: **10 mm ≤ D ≤ 30 mm**.

3. **Decision Logic**  
   - If ripe and within valid size range → signal `RIPE` (Arduino receives command to grip).  
   - Else → `UNRIPE` (Arduino does nothing).

---

### ▶️ How to Run

#### 1. Clone the repository
```bash
git clone https://github.com/Thibaultvrn/Computer-Vision-Rasberry-size
cd Computer-Vision-Rasberry-size
```

#### 2. Install OpenCV for C/C++

Follow the detailed guide in [INSTALL_OPENCV_C.md](INSTALL_OPENCV_C.md).

**Quick options:**
- **Windows (binaries)**: Download from https://opencv.org/releases/ and configure environment variables (~15 min)
- **Windows (WSL/Linux)**: `sudo apt install libopencv-dev` (~5 min, recommended)
- **Linux**: `sudo apt install libopencv-dev`

#### 3. Compile the program

**On Windows with Visual Studio:**
```cmd
build.bat
```

**On WSL/Linux:**
```bash
make
```

**Manual compilation (Windows):**
```cmd
cl /I"C:\opencv\build\include" Algorith.C /link /LIBPATH:"C:\opencv\build\x64\vc16\lib" opencv_world481.lib
```

**Manual compilation (Linux/WSL):**
```bash
gcc Algorith.C -o algorith `pkg-config --cflags --libs opencv4`
```

#### 4. Run the detection program

```bash
./algorith raspberry.jpg 0.5
```

Arguments:
- `image_path` – Path to the raspberry image
- `scale_mm_per_pixel` – Scale factor for converting pixels to millimeters (default: 0.5)

The program will:
- Display detection results in console (RIPE/UNRIPE, size, confidence scores)
- Show the image with detected contours and bounding boxes
- Press any key to close the windows

---

### 📁 Project Structure

```
Computer-Vision-Rasberry-size/
├── Algorith.C               # Main C program - raspberry detection with OpenCV C API
├── build.bat                # Windows compilation script (Visual Studio)
├── Makefile                 # Linux/WSL compilation script
├── INSTALL_OPENCV_C.md      # Detailed OpenCV installation guide for C/C++
├── README.md                # This file
├── LICENSE                  # Project license
└── docs/                    # Course documentation (ME-320 EPFL)
```

---

### 🎓 Course Documentation

This project is developed as part of EPFL's ME-320 course. Lecture materials are available in the `docs/` folder
