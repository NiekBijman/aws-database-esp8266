# Store RFID data in Amazon DynamoDB through the API gateway
<img src="/images/RFID_tags.v2.gif" alt="alt text" width="360" height="240">

## Hardware
#### Create the circuit
You can store any type of data that you can send over HTTPS to AWS. For my project I have chosen RFID tag identification. The components include the Adafruit ESP8266 feather board. It is arduino compatible and has libraries available for performing HTTPS requests. 

<img src="/images/Component-List-and-circuit.png" alt="alt text" width="717" height="369">



## Software
You have to set up an AWS account -> https://goo.gl/jzrnYe to send data to DynamoDB. Limited data storage fall under the AWS free tier which you can view here -> https://aws.amazon.com/free/ . If you are unsure if you fall in the free category you can check your billing console which keeps track of the costs for your project -> https://goo.gl/w36zHC. Here is a great walkthrough to set up the API gateway to forward data to DynamoDB.

Open the data-storage-dynamodb.ino file and place your Wifi SSID, password and AWS endpoint in the sketch. I'm using a local config file for this information so you can remove the line in the sketch to compile. 


<img src="/images/Prototype.png" alt="alt text" width="540" height="284">
