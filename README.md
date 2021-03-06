# Store RFID data in Amazon DynamoDB through the API gateway
<img src="/images/RFID_tags.v3.gif" alt="alt text" width="360" height="240">

This project sends RFID tag data using HTTPS to AWS services to store data in their database. This repository can be useful to you if you want to learn more about the HTTPS protocol, setting up an API yourself and sending data to your API. Furthermore you can see examples for working with RFID data and libraries for creating JSON strings in Arduino. 

## Overview

<img src="/images/Project-Infrastructure.png" alt="alt text" width="480" height="142">

For this project you need:
* An AWS account.
* The BOM-list specified in *Hardware*.

The API gateway and DynamoDB will take time to set up. Here is a great walkthrough to set up the API gateway to forward data to DynamoDB -> https://www.youtube.com/watch?v=TuGyyTXPQ-U



## Hardware
#### Create the circuit
You can store any type of data that you can send over HTTPS to AWS. For my project I have chosen RFID tag identification. The components include the Adafruit ESP8266 feather board. It is arduino compatible and has libraries available for performing HTTPS requests. The other components that are used are the RC522 RFID board, an LED and buzzer for user feedback. 

You can see the components used in this project in this link: 
https://www.circuito.io/app?components=513,8662,360216,761981

<img src="/images/Component-List-and-circuit.png" alt="alt text" width="717" height="369">



## Software
You have to set up an AWS account -> https://goo.gl/jzrnYe to send data to DynamoDB. Limited data storage fall under the AWS free tier which you can view here -> https://aws.amazon.com/free/ . If you are unsure if you fall in the free category you can check your billing console which keeps track of the costs for your project -> https://goo.gl/w36zHC. 

Arduino 
* Open the data-storage-dynamodb.ino file 
* Get the ESP8266  core for arduino in the Library Manager
* In the Serial Monitor set the Baudrate to *115200* and select your ESP8266 board in *tools* -> *board* . These settings make sure that you can read the data that's being printed to the Serial monitor. 
* Enter your **wifi-SSID**, **password** and **AWS URL** & **AWS Endpoint** from AWS in the .ino file.  I'm using a local config file for this information so you can remove the line in the sketch to compile. 


<img src="/images/Prototype.png" alt="alt text" width="540" height="284">
