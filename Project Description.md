# Smart Door Lock using Esp32 and Esp32-Camera
`Author` : Pham Hong Manh - Students majoring in Mechatronics K63 at Hanoi University of Science and Technology

`Reasons for choosing the topic` : The demand for smart door locks is increasing, so I decided to choose the topic of Smart door locks using Esp32

`Copyright:` You can use the project but please cite the source when you use this project of mine.

The project may have many shortcomings, I hope to receive your comments

# 1. Overview


When entering the correct fingerprint or password, the door lock will be unlocked. The parameters will be displayed on the LCD screen

The lock has 3 modes: change password, add fingerprint and remove fingerprint
The project is continuing to be researched to be able to connect to the network to display information such as time, weather, attendance, and remote opening.

The components used in the project include: 
+ 4x4 matrix keyboard
+ 1.8 inch lcd screen
+ AS608 fingerprint module
+ Esp32 microcontroller and esp32-camera
# 2. Hardware Connection Diagram
<img src="https://user-images.githubusercontent.com/95463241/210167944-556747a4-3212-403e-9d20-c8d30c6a2cc2.png" width = "750">

# 3. Note
You need to edit the TFT library so don't forget to read file README.md 
