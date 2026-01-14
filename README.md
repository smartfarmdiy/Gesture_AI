# Gesture_AI

AI Gesture-Driven Smart Trash Bin Robot
Built with Grove Vision AI Module V2 and Seeed Studio XIAO ESP32S3

SenseCraft AI platform

Key Steps:
Sign in to the SenseCraft AI platform
1 Sign in to the SenseCraft AI platform
Choose a compatible gesture recognition model
1 Choose a compatible gesture recognition model
Flash the model to Vision V2 via USB-C
1 Flash the model to Vision V2 via USB-C
Monitor real-time detection results through the preview interface
1 Monitor real-time detection results through the preview interface
The biggest advantage of this approach is accessibility—no complex training pipeline or deep AI knowledge is required. The visual feedback makes debugging fast and intuitive.
Once a gesture is detected, Vision V2 sends structured recognition data to the main controller over I²C communication.
Embedded Control Layer
The XIAO ESP32S3 serves as the central processing unit of the robot. Its tasks include:
Receiving recognition data from Vision V2
1 Receiving recognition data from Vision V2
Decoding the transmitted JSON information
1 Decoding the transmitted JSON information
Determining the appropriate action
1 Determining the appropriate action
Driving motors and controlling the servo
1 Driving motors and controlling the servo
The Grove Shield simplifies wiring and ensures compatibility across the XIAO series. Development is done using the Arduino framework, which provides flexibility and rapid iteration.
Development Environment
Software Stack:
VS Code
1 VS Code
PlatformIO
1 PlatformIO
Board: Seeed Studio XIAO ESP32S3
1 Board: Seeed Studio XIAO ESP32S3
Framework: Arduino
1 Framework: Arduino
To improve readability and maintainability, the firmware is divided into independent modules:
AI data handling
1 AI data handling
Motion control
1 Motion control
Servo actuation
1 Servo actuation
The complete project structure and source code are available in my GitHub repository: 🔗 Gesture_AI
Gesture Logic Processing
The AI module outputs recognition results in JSON format. These results are parsed by a dedicated gesture-handling module, which maps gestures to actions.
Example gesture mapping:
Rock → Robot advances forward
1 Rock → Robot advances forward
Paper → Robot reverses direction
1 Paper → Robot reverses direction
Scissors → Trash bin lid opens or closes
void handle_target_0(const boxes_t *box); // Forward motion void handle_target_1(const boxes_t *box); // Reverse motion void handle_target_2(const boxes_t *box); // Servo action
This design allows new gestures or behaviors to be added with minimal changes.
Motion Control System
The robot uses four DC motors to move smoothly. Each motor is controlled through GPIO pins via motor driver modules.
#define motor_1_A D10 #define motor_1_B D9 #define motor_2_A D8 #define motor_2_B D7 #define motor_3_A D0 #define motor_3_B D1 #define motor_4_A D2 #define motor_4_B D3 void motor_init(){} void motor_stop(){} void motor_forward(){} void motor_backward(){}
By switching pin states, the motors can be enabled, stopped, or reversed to control direction.
Servo Actuation
A servo motor is responsible for opening and closing the trash bin lid. PWM output is used to define the rotation angle.
void servo_set_angle(int angle) { angle = constrain(angle, 0, 180); int duty = map(angle, 0, 180, 51, 102); ledcWrite(SERVO_CHANNEL, duty); }
This method ensures stable and precise lid movement.
Vision Communication Library
The project relies on SSCMA, Seeed Studio’s official communication library, to manage data exchange between the microcontroller and the Vision AI module efficiently.
Enclosure & Mechanical Design
All structural components and covers were designed using Onshape, a cloud-based CAD platform.
Benefits:
Runs directly in the browser
1 Runs directly in the browser
No installation required
1 No installation required
Easy to iterate and modify designs
1 Easy to iterate and modify designs
Once printed, the enclosure sits on top of the trash bin, hiding cables and giving the robot a more finished look.
Wiring & Assembly Notes
Due to the number of components involved, wiring requires extra care. Incorrect connections may damage the motor drivers or microcontroller.
Tips:
Double-check polarity and pin assignments
1 Double-check polarity and pin assignments
Test each subsystem independently
1 Test each subsystem independently
Consider designing a custom PCB for future revisions
1 Consider designing a custom PCB for future revisions
A proper enclosure significantly improves both reliability and appearance.
Results & Reflections
After programming and testing, the robot successfully:
Detects hand gestures in real time
1 Detects hand gestures in real time
Moves based on user input
1 Moves based on user input
Opens the trash bin lid without physical contact
1 Opens the trash bin lid without physical contact
This project is still a work in progress, but every experiment adds new insights. If you have ideas for improvements—better motion control, additional gestures, or smarter behavior—I’d be excited to hear them.
