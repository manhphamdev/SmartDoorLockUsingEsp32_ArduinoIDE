# Smart Door Lock using Esp32 and Esp32-Camera

## HOW TO ADD LIBRARY:
The libraries folder contains all the libraries that I use in this project (there are a few libraries I don't use). 
You can add the library to your project by following steps:

`Step 1`: Save the library folder to the Arduino IDE's Libraries drive. Usually located at tr: This PC/Documents/Adruno

`Step 2`: Replace the old library with the unzipped library.

**you need to notice library folder of 1.8 inch tft lcd monitor** :

Go to folder: TFT_eSPI. Then open folder: User_Setup.h. Here you need to reconfigure the connection pins of the tft lcd screen to match the hardware connection diagram. I already config so if you use same monitor and microcontroller as me you can use directly. If you are not using the same type of ESP 32 microcontroller you can reconfigure it.

If you find it difficult to set up the TFT_eSPI library, you can read the README.md file in the TFT.eSPI or refer to the following video: https://youtu.be/WFVjsxFMbSM
