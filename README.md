<div align="center">
 
## oneTouchCozyMood

 <div align="center">
  
  MSc Team of University of Glasgow - Real Time Embedded Programming (2021-22) (ENG5220)
 
  
  
  
   
  
 ![](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/IMG_0050.jpg)
 ----
  
  
  
## **Significant sites**
  
  
  <div align="left">
   
  
  
   ![](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/kiss-face-emoji.jpg)[More about our oneTouchCozyMood!](https://github.com/Nishant-web/oneTouchCozyMood)
  
   ![](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/emoticon.jpg)Check our [wiki](https://github.com/Nishant-web/oneTouchCozyMood/wiki) to understand more!!
  
   ![](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/thumbup_good_emoticon-512.jpg)Watch [Video Demonstration](https://www.youtube.com/channel/UCGz5qoQ1cUQMT-c8PuJZ9TA) of our project here!!!!
  
   To analyze more, browse [hackaday.io](https://hackaday.io/project/184873-one-touch-cozy-mood)!!!
  
 -----
   
 <div align="left">
 
 ## **Table of Contents**
 
 <div align="left">
  
 1. [Project Description](#project-description) <br/>
 2. [Hardware and Software used](#hardware-and-software-used) <br/>
 3. [Project Flowchart](#flow-diagram) <br/>
 4. [Project Demo](#project-demo) <br/>
 5. [Quick Guide](#quick-guide) <br/>
 6. [Future Scope](#future-scope) <br/>
 7. [License](#license) <br/>
 8. [Social Media Channel](#social-media) <br/>
 9. [Authors](#authors) <br/>
 
  
  -----
  
  
  <div align="center">
   
 
 ## **PROJECT DESCRIPTION**
 
The project aims to deliver a real-time output of a system by using raspberry pi in object oriented C++ language, we are interfacing fingerprint sensor (R305), speaker via Gpio ports, tactile switches and Bluetooth connectivity respectively. This paradigm is brought into existence to lighten-up user's moods especially after a long day at work, by adjusting the ambiance according to their mood with just one touch, hence the name "oneTouchCozyMood" which came after a lot of contemplating on several catchy names and this seemed apt for the project and its promotion on social media. The logo displayed below strives to show explicitly what the project is really about and goes with the name as a classic medley of both name and logo, which gives the user relaxing and fun vibes at first glance.

-----

   
  ## **HARDWARE AND SOFTWARE USED**
 
 <div align="left">
 1. Fingerprint sensor R305 <br/>
 2. RGB LED Light  <br/>
 3. Tactile switches  <br/>
 4. Bluetooth speaker  <br/>
 5. Embedded C++ Programming <br/>
 6. Raspbian OS <br/>
 
 -----

  <div align="center">
  
## **QUICK GUIDE** 

  <div align="left">
   
  
   To start this project
 
   ^ After downloading the zip, you will see oneTouchCozyMood folder in that, there are five folders: 1) Data, 2) Delete_Users, 3) Enroll_Users, 4)Music_Player, 5)Unit_Test <br/>
   ^ Run below commands to download our source code and build it <br/>
   ^ Then, look into our circuit design as shown below. Also, you can refer pictures and videos for connecting components <br/>
   ^ Open unit test folder(use for checking the working of hardware) -- Click on build -- Then click on enroll ---then click on execute in terminal <br/>
   ^ Open Enroll folder -- Click on Build-- Then click on enroll ---then click on execute in terminal <br/>
   ^ Enter ID, name and scan your finger on fingerprint sensor then Your fingerprint name will be registered in datafolder <br/>
   ^ Add your favourite playlist to your folder <br/>
   ^ Go to Music player and click on build --- then click on Play --- click on execute in terminal  <br/>
   ^ Scan your registered finger and then ambient light and your playlist will start <br/>
*IGNORE readme.txt word document in all build folders*   

  To download our source code along with submodule

```bash
  $ git init 
  $ git clone https://github.com/Nishant-web/oneTouchCozyMood.git
  $ cd oneTouchCozyMood/
  $ git submodule update --init --recursive --remote 
```
To download pre-build libraries

```bash
  $ chmod 0770 prerequisites.sh
  $ ./prerequisites.sh
```
To build

```bash
  $ cd oneTouchCozyMood/
  $ cd Delete_Users/build/
  $ cmake ..
  $ make all
  $ cd ../../Enroll_Users/build/
  $ cmake ..
  $ make all
  $ cd ../../Music_Player/build/
  $ cmake ..
  $ make all
  $ cd ../../Unit_Test/build/
  $ cmake ..
  $ make all
```  
<div align="center">
 
 ----- 
 
## **FLOW DIAGRAM**
   
  ![](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/WhatsApp%20Image%202022-07-28%20at%2010.29.43%20PM.jpeg)

  
 <div align="center"> 
 
  -----

## **CIRCUIT DESIGN** 

![](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/IMG-20220729-WA0005.jpg)
 
 -----
<div align="center">
 
## **PROJECT DEMO**  
  Watch our project [VIDEO DEMO](https://www.youtube.com/watch?v=O_imgMwheiM) here.

  ----- 

<div align="center">
     
## **FUTURE SCOPE**
  
   <div align="left">
  In the long run the updates for this device will include more accessibility for the user in terms of remote control access such as smartphone application to use smartphones fingerprint sensor to be able to switch things with just one touch for instance, connect a TV via bluetooth connectivity, however different fingre print data will be allocated to a specific user, so that one will be able to turn on TV with one finger print and put on the relaxing ambience with the other one from the application itself.
  
  <div align="center">
   
   -----
   
## **SOCIAL MEDIA**

<div align="center">
 
 [![instagram](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a5/Instagram_icon.png/50px-Instagram_icon.png)](https://www.instagram.com/one_touch_cozy_mood/)
 [![twitter](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/Twitter.jpg)](https://twitter.com/touch_cozy_mood)
 [![youtube](https://github.com/Nishant-web/oneTouchCozyMood/blob/main/Logos/Youtube%20(2).jpg)](https://www.youtube.com/channel/UCGz5qoQ1cUQMT-c8PuJZ9TA)
 
   -----
   
 
## **LICENSE**
<div align="left">
 
 The information about the license [MIT](https://choosealicense.com/licenses/mit/) is given here.
 
  
 
  -----
  
<div align="center"> 

 ## **AUTHORS**
 
<div align="left">
  
   * [Nishant Kshirsagar](https://github.com/Nishant-web)   </br>
   * [Amit Rajendra Relan](https://github.com/amitrelan238) </br>
   * [Chasmitha Vadlamudi](https://github.com/Chasmithav)   </br>
   
   * Any queries, email us at [onetouchcozymood@gmail.com](https://outlook.office365.com/mail/deeplink/compose?mailtouri=mailto%3Aonetouchcozymood%40gmail.com)

  
  -----
   
  © 2022 Copyright UofG_Real_Time_Embedded_Systems_oneTouchCozyMood_Team 2
   
 <div align="center">
  
  [![Contributors](https://img.shields.io/github/contributors/andretsolkas/oneTouchCozyMood.svg?style=for-the-badge)](https://github.com/andretsolkas/oneTouchCozyMood/graphs/contributors)
  [![Forks](https://img.shields.io/github/forks/andretsolkas/oneTouchCozyMood.svg?style=for-the-badge)](https://github.com/andretsolkas/oneTouchCozyMood/network/members)
  [![Stars](https://img.shields.io/github/stars/andretsolkas/oneTouchCozyMood.svg?style=for-the-badge)](https://github.com/andretsolkas/oneTouchCozyMood/stargazers)
  [![Issues](https://img.shields.io/github/issues/andretsolkas/oneTouchCozyMood.svg?style=for-the-badge)](https://github.com/andretsolkas/oneTouchCozyMood/issues)
  [![License](https://img.shields.io/github/license/andretsolkas/oneTouchCozyMood.svg?style=for-the-badge)](https://github.com/andretsolkas/oneTouchCozyMood/blob/main/LICENSE)
  
</div>

<div align="center">
  
  <a href="https://github.com/andretsolkas/oneTouchCozyMood/issues">Report Bug</a>
  ·
  <a href="https://github.com/andretsolkas/oneTouchCozyMood/issues">Request Feature</a>
  
</div>
