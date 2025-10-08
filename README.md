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
| **Computer Vision (Python)** | Detects fruit ripeness and size using OpenCV | Python 3 |

Data is exchanged through the **serial interface** (`PySerial`).

---

### ⚙️ Technologies Used
- **Python 3**
- **OpenCV** – Image capture & color segmentation
- **NumPy** – Matrix operations
- **PySerial** – Communication with Arduino
- **Arduino C/C++** – For actuator control

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

#### 2. Install dependencies
```bash
pip install -r requirements.txt
```

This will install:
- **opencv-python** – Computer vision library
- **numpy** – Numerical computing
- **pyserial** – Arduino communication

#### 3. Run the detection program
```bash
python raspberry_detector.py raspberry.jpg --scale 0.5
```

Arguments:
- `image_path` – Path to the raspberry image
- `--scale` – Scale factor in mm/pixel (default: 0.5)
- `--no-display` – Run without GUI (batch mode)

For detailed installation instructions, see [INSTALL.md](INSTALL.md).

---

### 📁 Project Structure

```
Computer-Vision-Rasberry-size/
├── raspberry_detector.py    # Main Python detection program
├── Algorith.C               # Alternative C implementation (requires OpenCV C API)
├── requirements.txt         # Python dependencies
├── INSTALL.md              # Detailed installation guide
├── README.md               # This file
├── LICENSE                 # Project license
└── docs/                   # Course documentation
```

---

### 🎓 Course Documentation

This project is developed as part of EPFL's ME-320 course. Lecture materials are available in the `docs/` folder
