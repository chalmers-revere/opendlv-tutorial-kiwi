# Getting Started with the Kiwi Platform

This "Getting Started" tutorial will introduce you to Kiwi, the miniature robotic vehicle platform from Chalmers Revere. Its hardware and software are entirely open source and you are invited to use, change, and contribute.

*Table of Contents*
* [Introducing Kiwi](#introducing-kiwi)
* [Working with the Kiwi hardware](#working-with-the-kiwi-hardware)
* [Working with data replay for Kiwi](#working-with-data-replay-for-kiwi)
* [Working with Kiwi simulation](#working-with-kiwi-simulation)
* [Next steps](#next-steps)

---
## Introducing Kiwi

The following components are installed on your Kiwi:

1. BeagleBone Blue board
2. Raspberry Pi 3 mod B board
3. Battery (LiPo 7.4V 1200mAh 30C)
4. Electronic Speed Control (ESC) unit (SKYRC Cheetah 60A Brushless ESC)
5. Steering servo (Hitec Midi-Servo HS-5245MG)
6. Motor (SKYRC Cheetah Brushless ESC)
7. Ultrasonic sensors (front and rear) (Devantech SRF08)
8. Infrared sensors (left and right) (Sharp GP2Y0A41SK0F)
9. Raspberry Pi Camera Module v2

The following pictures provide an overview of the installed components on Kiwi.
![top](https://github.com/chalmers-revere/opendlv-tutorial-kiwi/raw/master/kiwi-front.png)
![bottom](https://github.com/chalmers-revere/opendlv-tutorial-kiwi/raw/master/kiwi-bottom.png)

All the sensors except for the camera are connected to a small PCB board and then to the BeagleBone Blue board. The camera is directly connected to the Raspberry Pi board. The two boards are connected by a USB cable, which is treated by both boards as a standard network connection. Both boards are also wireless-ready: in particular, the Raspberry Pi has WiFi capabilities, while the BeagleBone Blue has both WiFi and Bluetooth connectivity.

### Booting the system
There is a specific sequence to follow in order to boot up the Kiwi platform correctly. First, use the main power switch to turn on the BeagleBone Blue. You will see several blue led lights blinking. Wait until the green leds in the bottom-right corner of the board are turned on. At this point, you can turn on the ESC by pushing its power button: you will start hearing a repetitive beep sound that indicates that the beaglebone is waiting to connect to the raspberry pi. Wait until you see a red led on in the top right corner of the beaglebone. Finally push the following buttons in sequence: MOD, PAU, MOD. The beeping sound should stop indicating that the system has started correctly.

### Connect to Kiwi

Kiwi comes with Wifi enabled by default and is configured to share an Internet connection. Simply search in your Wifi settings for the access point (AP) that is stated on the sticker on the car and connect to that AP (password: BeagleBone). Your laptop should receive an IP address within the range `192.168.8.x/24`. Once you have such an IP address, you can `ping` the computers on Kiwi or connect via `ssh` using the following credentials for beaglebone (user: debian, password: temppwd) and pi (user:pi, password: raspberry).

Kiwi's Raspberry Pi is running [Rasbian Stretch](https://www.raspberrypi.org/downloads/raspbian/) and BeagleBone Blue is running [Debian 9 (stretch)](https://debian.beagleboard.org/images/bone-debian-9.2-iot-armhf-2017-10-10-4gb.img.xz).

To ssh to the beaglebone
`ssh debian@192.168.8.1`
To ssh to the pi
`ssh -p 2200 pi@192.168.8.1`

### Interacting with Kiwi using Microservices

Kiwi's software is encapsulated into separate [microservices](https://en.wikipedia.org/wiki/Microservices) that interact using UDP multicast. An overview of available microservices in use at Chalmers Revere is provided [here](https://github.com/chalmers-revere/opendlv).

The tutorials below will show how to both use microservices on the Kiwi itself, and how to connect to microserices running on an external computer, as for example a laptop.

---

## Working with the Kiwi hardware

### Getting Started 1.1: Camera Live Stream to a Webbrowser, data recording, data replay, data export

The first tutorial is to run microservices that stream the camera feed to your web browser. For this tutorial, we need the following three microservices running on *Raspberry Pi*:

1. [opendlv-device-camera-rpi](https://github.com/chalmers-revere/opendlv-device-camera-rpi) - accessing Raspberry Pi's camera
2. [opendlv-video-x264-encoder](https://github.com/chalmers-revere/opendlv-video-x264-encoder) - converting a camera frame into an h264 frame
3. [opendlv-kiwi-view](https://github.com/chalmers-revere/opendlv-kiwi-view) - web application to visualize, record, and replay data

_Overview:_

The first microservice is opening the camera and reading the images into two shared memory areas - one contains the frame in [I420](https://wiki.videolan.org/YUV/#I420) pixel format and the other contains the frame in ARGB pixel format. While the latter might be more convenient to be used with image detection algorithms, the former is used for h264 encoding.

The second microservice is attaching to the shared memory area that contains the I420-formatted pixels and encodes each frame into an [opendlv.proxy.ImageReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/40f0cdb83632c3d122d2f35e028331494313330f/opendlv.odvd#L150-L155) message that contains the h264-encoded frame. This message is broadcasted to other microservices.

The third microservice is providing a web-application to visualize, record, and replay any messages that are exchanged among the running microservices.

To simplify the distribution of microservices and the description how to configure and run them, we are using Docker. The listed microservices are pre-configured [here](https://raw.githubusercontent.com/chalmers-revere/opendlv-tutorial-kiwi/master/rpi-camera-x264-viewer-kiwi.yml) and already running on Kiwi.

_Getting Started:_

Now, connect your laptop's web browser to the *Raspberry Pi's* IP address, port 8081: http://192.168.8.1:8081. As a result, you should see a live stream from your Kiwi's camera and the installed sensors visualized in your web browser.

To record the live data, simply click on the record button that should turn red. You can stop the recording by pressing that button again. To download a recording, click on the folder button next to the record button; a new page is presented that shows the available recording files that reside on Kiwi. You can select the file you would like to download by clicking on the button labeled with `.rec`. If you want to export the recorded messages a separate `.csv`, simply click on the button with the corresponding label. To delete a recording, click on the trash bin and to start a replay, simply click on the replay button.

### Getting Started 1.2: Controlling Kiwi using your web browser

The second tutorial is start an additional microservice to control Kiwi with your web browser. For this tutorial, we run all microservices from *Tutorial 1.1* and add the following microservice on *BeagleBone Blue*:

* [opendlv-device-kiwi-prugw](https://github.com/chalmers-revere/opendlv-device-kiwi-prugw) - interfacing with Kiwi's motor and servo

_Overview:_

This microservice is listening for the messages [opendlv.proxy.PedalPositionRequest](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L208-L210) and [opendlv.proxy.GroundSteeringRequest](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L216-L218) to interface with Kiwi's motor and servo for acceleration/deceleration and steering.

To simplify the distribution of this microservice and the description how to configure and run it, we are using Docker. The listed microservices are pre-configured and already running on Kiwi.

_Getting Started:_

Now, connect your laptop's web browser to the *Raspberry Pi's* IP address, port 8081: [http://192.168.8.1:8081](http://192.168.8.1:8081). You should still see a live stream from your Kiwi's camera visualized in your web browser. Next, enable the `Joystick` by pushing the joystick button in order to send [opendlv.proxy.PedalPositionRequest](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L208-L210) and [opendlv.proxy.GroundSteeringRequest](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L216-L218) to interface with Kiwi's motor and servo. Now, you can click and drag with your mouse (or pan on a smartphone/tablet) to accelerate/decelerate and steer Kiwi.

---

## Working with data replay for Kiwi

### Getting Started 2.1: Local replay on your computer

This tutorial is to download a recording file and replay it on your local system. For this test, we assume that you have completed tutorial 1.1.

_Prerequisites:_

* [You need to install Docker for your platform](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
* [You need to install `docker-compose`](https://docs.docker.com/compose/install/#install-compose)
* Recording files including video

_Tutorial:_

* Download the `.rec` file of interest from Kiwi to your computer into the folder `$HOME/kiwi-recordings`.
* Next, download the application description (a .yml-file for docker-compose) to `$HOME` in a terminal: `wget https://raw.githubusercontent.com/chalmers-revere/opendlv-tutorial-kiwi/master/opendlv-perception-helloworld-python/h264-replay-viewer.yml -O h264-replay-viewer.yml`.
* Edit the YML file so that `yourFile.rec` one of your recording files. List your file names in the terminal by running `ls -l $HOME/kiwi-recordings` running in a terminal.
* Next, in the terminal, run the command `xhost +` to allow Docker to access the desktop environment (i.e. opening new windows on the display). This needs to be done once everytime you restart your computer.
* Now, start the application description: `docker-compose -f h264-replay-viewer.yml up` in a terminal. The first time this is done, the h264-replay program will be compiled (due to licencing reasons).

A rendering of recorded video will be show on screen, and the web viewer will give the following message in your terminal: `[opendlv-vehicle-view] Web server listening on port: 8081, joining live OD4Session 112, using OD4Session 253 for playback.`

Now, you can open a modern web browser and point it to http://localhost:8081. You should now be able to see the messages sent over UDP multicast in the messages tab. You can also use the interface to record messages, and any such new recordings will end up in the folder `$HOME/recordings`. Note however that the video will not be resaved in the new recording file, since it is transmitted via shared memory (and not on UDP multicast). Please create such recording file for the next step of the tutorial.

You can stop the application by pressing `Ctrl-C` followed by the command `docker-compose -f h264-replay-viewer.yml down` in a terminal.

Note that the web app requires quite much from the computer to run. Here we used it to see the UDP messages, and to make a record file, but the messages will be available in the running session also without the web app. To start only the replay microservice, run `docker-compose -f h264-replay-viewer.yml up video-h264-replay-amd64`.

### Getting Started 2.2: Extracting `.csv` files from a recording on your computer

This tutorial is to extract `.csv` files from a recording file for data post-processing. A `.csv` file is create for every message type that is exchanged on Kiwi including the sent, received, and sample timestamps.

_Prerequisites:_

* Have tutorial 2.1 completed.

_Tutorial:_

* Start the application description: `docker-compose -f h264-replay-viewer.yml up vehicle-view` in a terminal. You do not need to start the replay microservice this time.

The application is available when you read a message stating `[opendlv-vehicle-view] Web server listening on port: 8081, joining live OD4Session 112, using OD4Session 253 for playback.`

Now, you can open a modern web browser and point it to http://localhost:8081. First, go to the folder view where you should see the recording you made earlier. Clicking on the name of the file, and then the messages tab (for that file), one can see that there are some unknown messages in the file. These messages needs a newer version of the OpenDLV standard message set to be displayed properly, so we need to add it to the web app.

Download the latest file [here](https://raw.githubusercontent.com/chalmers-revere/opendlv.standard-message-set/v0.9.10/opendlv.odvd). Save it to your Downloads folder or similar, then, by using the Linux file browser, drag the file into the top dashed area inside the vehicle view web page. This will enable all messages inside the recording file. 

Then, go back to the folder view and click on the button labeled `.csv`. This will extract all messages in separate `.csv` files, and offer a download for a `.zip`-file containing all files. Depending on the size of the selected recording file, this step might take up to a few minutes.

You can stop the application by pressing `Ctrl-C` followed by the command `docker-compose -f h264-replay-viewer.yml down` in a terminal.

---

## Working with Kiwi simulation

### Getting Started 3.1: Starting the simulation environment

This tutorial is on starting a Kiwi car simulation, with a simulated camera, and
then to steer ut using the vehicle view. For this test, we assume that you have completed tutorial 1.2.

_Prerequisites:_

* [You need to install Docker for your platform](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
* [You need to install `docker-compose`](https://docs.docker.com/compose/install/#install-compose)

_Tutorial:_

* Download the application description (a .yml-file for docker-compose) to `$HOME` in a terminal: `wget https://raw.github.com/chalmers-revere/opendlv-tutorial-kiwi/master/simulation-kiwi.yml`
* Download the cone track scenario to `$HOME` in a terminal: `wget https://raw.github.com/chalmers-revere/opendlv-tutorial-kiwi/master/conetrack.zip`
* Uncompress the zip file by running the command `unzip conetrack.zip`.
* Next, in the terminal, run the command `xhost +` to allow Docker to access the desktop environment (i.e. opening new windows on the display). This needs to be done once everytime you restart your computer.
* Now, start the application description: `docker-compose -f simulation-kiwi.yml up` in a terminal.

You can stop the simulation at any time by pressing `Ctrl-C` followed by the command `docker-compose -f simulation-kiwi.yml down` in a terminal.

Note that the default version is for Intel GPUs and VirtualBox. If you want to run the simulation using an Nvidia GPU, change `chalmersrevere/opendlv-sim-camera-mesa:v0.0.1` into `chalmersrevere/opendlv-sim-camera-nvidia:v0.0.1` in the .yml file. 

Note also that if using a VirtualBox for this tutorial, the graphics rendering will be done using a software renderer resulting in slow rendering. If given the message `[cluon::OD4Session]: time-triggered delegate violated allocated time slice.`, then the simulation cannot keep up. To solve this, modify the .yml file and change the `--timemod` arguments for the three simulated components from `1.0` to `0.2`. **NOTE:** It needs to be changed for all *three* simulation services.

A rendering of the simulated camera from the virtual Kiwi car will be show on screen, and the web viewer will give the following message in your terminal: `[opendlv-vehicle-view] Web server listening on port: 8081, joining live OD4Session 111, using OD4Session 253 for playback.`

Now, you can open a modern web browser and point it to http://localhost:8081. You should now be able to see the messages sent over UDP multicast in the messages tab. You can also use the interface to record messages, and any such new recordings will end up in the folder `$HOME/recordings`. Note however that the video will not be resaved in the new recording file, since it is transmitted via shared memory (and not on UDP multicast). Next, enable the `Joystick` by pushing the joystick button in order to send [opendlv.proxy.PedalPositionRequest](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L208-L210) and [opendlv.proxy.GroundSteeringRequest](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L216-L218) to interface with Kiwi's motor and servo. Now, you can click and drag with your mouse (or pan on a smartphone/tablet) to accelerate/decelerate and steer the simulated Kiwi.

Note that the simulated video seems to be rather low framerate, and if using the `--timemod` option this is even more visible. This is intended, as the Raspberry Pi camera has an update frequency of 7.5 FPS (which might be lowered with the time modifier).

### Getting started 3.2: Driving two simulated Kiwi cars in the same simulation

This tutorial is on starting a second Kiwi car in the same simulation as demonstrated in 3.1.
For this test, we assume that you have completed tutorial 3.1.

_Tutorial:_

* Open a second terminal.
* Download the application description, as a complement to the one downloaded in 3.1, to `$HOME` in a terminal: `wget https://raw.github.com/chalmers-revere/opendlv-tutorial-kiwi/master/simulation-kiwi-two.yml`
* Make sure that both files use the same `--timemod` value on _all_ simulation services
* Now, in the first terminal, start the same application as in 3.1 by running `docker-compose -f simulation-kiwi.yml up` and, in the second terminal, run `docker-compose -f simulation-kiwi-two.yml up`

This will run a second simulated Kiwi car isolated in its own OpenDLV context (a libcluon conference). The only data shared between the two contexts are  [opendlv.sim.Frame](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L19-L26), which is used for integrating the two simulations.

Open two browser tabs and point them to http://localhost:8081 and http://localhost:8082 (one for each simulated Kiwi car). Next, enable the `Joystick` by pushing the joystick button in order to steer the simulated Kiwi. Turn around in order to see the second Kiwi car and then stop. Now, switch to the second browser tab and enable the `Joystick` to then steer the second Kiwi to see it move in the simulated camera stream.

---

## Next steps

_Where to go from here?_

Now, you are able to use the camera from your Kiwi, visualize, record, and replay the data, and to send steering, acceleration, and deceleration commands.

Next, you can investigate our template module that is running in Python to process video data and to send steering, acceleration, and deceleration commands to Kiwi. You find the template [here](https://github.com/chalmers-revere/opendlv-tutorial-kiwi/tree/master/opendlv-perception-helloworld-python) next to further descriptions how to develop and test your Python application.

Alternatively, you can investigate our template module that is running in C++ to process video data and to interface with Kiwi. You find the template [here](https://github.com/chalmers-revere/opendlv-tutorial-kiwi/tree/master/opendlv-perception-helloworld-cpp) next to further descriptions how to develop and test your C++ application.
