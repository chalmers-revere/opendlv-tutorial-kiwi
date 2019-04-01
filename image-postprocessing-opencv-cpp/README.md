## Processing image data with OpenCV and controlling Kiwi using C++

This C++-template demonstrates how to develop a software module to process video data (eg., image detection, etc.) to be used with Kiwi.

Prerequisites:
* [You need to install Docker for your platform](https://docs.docker.com/install/linux/docker-ce/debian/#install-docker-ce)
* [You need to install `docker-compose`](https://docs.docker.com/compose/install/#install-compose)
* You have successfully completed the "Getting Started" tutorials [here](https://github.com/chalmers-revere/opendlv-tutorial-kiwi/tree/master).
* You have a recording file (`.rec`) with some video frames.

---

### Building and testing the software module for your laptop (replay mode)

This template folder contains everything you need to compile and run a C++ software component that uses OpenCV for image processing. We are using Docker to build and run the resulting binary.

* Step 1: Assuming that you have a folder `~/recordings`, where you have at least one `.rec` file from your experiments with Kiwi.

* Step 2: Clone this repository:
```bash
cd $HOME
git clone https://github.com/chalmers-revere/opendlv-tutorial-kiwi.git
cd opendlv-tutorial-kiwi/templates/image-postprocessing-opencv-cpp
```

* Step 3: Now, you can start the h264 decoder and webapp for replaying as follows (the actual h264 decoder is built once during the first call):
```bash
docker-compose -f h264-decoder-viewer.yml up
```

* Step 4: Next, start a webbrowser and connect to your local webapp: http://localhost:8081 and open the folder view. Select one of the `.rec` files for replay and begin the replay to fill the shared memory with some image data; you can pause the replay shortly after you saw the image in your browser.

* Step 5: Next, you enable access to your X11 server (GUI; necessary once per login):
```bash
xhost +
```

* Step 6: Assuming that you are located in the `image-postprocessing-opencv-cpp` folder, you can build the software module as follows:
```bash
docker build -t myapp -f Dockerfile.amd64 .
```

* Step 7: Now, you can run your software component:
```bash
docker run --rm -ti --init --net=host --ipc=host -v /tmp:/tmp -e DISPLAY=$DISPLAY myapp --cid=253 --name=img.argb --width=640 --height=480 --verbose
```

The application should start and wait for images to come in. Now, continue to replay your recording; the C++ application should open a new window and display the frames. Furthermore, the code also display the distance readings from the sensors.

You can stop your software component by pressing `Ctrl-C`. When you are modifying the software component, repeat step 6 and step 7 after any change to your software.

After a while, you might have collected a lot of unused Docker images on your machine. You can remove them by running:
```bash
for i in $(docker images|tr -s " " ";"|grep "none"|cut -f3 -d";"); do docker rmi -f $i; done
```

---

### Deploying and testing the Python application on Kiwi

When you are ready to test the features and performance of your software component on Kiwi in live mode, you need to build the software component for `armhf`. Therefore, you will find a file named `Dockerfile.armhf` in this template folder that describes the necessary steps to build your software component for `armhf`.  

* Step 1: Have the previous tutorial completed.

* Step 2: **Make sure that you commented all debug windows from OpenCV as there won't be a GUI available on Kiwi.''

* Step 3: Assuming that you are located in the `image-postprocessing-opencv-cpp` folder, you can build the software module for `armhf` as follows:
```bash
docker build -t myapp.armhf -f Dockerfile.armhf .
```

* Step 4: After having successfully built the software component and packaged it into a Docker image for `armhf`, you need to transfer this Docker image from your laptop to Kiwi. Therefore, you save the Docker image to a file:
```bash
docker save myapp.armhf > myapp.armhf.tar
```

* Step 5: Next, you copy the image to Kiwi's *Raspberry Pi* using secure copy (`scp`):
```bash
scp -P 2200 myapp.armhf.tar pi@192.168.8.1:~
```

* Step 6: Afterwards, you log in to Kiwi's *Raspberry Pi* and load the Docker image:
```Bash
ssh -p 2200 pi@192.168.8.1
cat myapp.armhf.tar | docker load
```

* Step 7: Assuming that the Getting Started Tutorial 2 (Controlling Kiwi using your webbrowser) is still running, you can finally run your software component next to other microservices on Kiwi's *Raspberry Pi*:
```bash
docker run --rm -ti --init --net=host --ipc=host -v /tmp:/tmp myapp.armhf --cid=112 --name=img.argb --width=640 --height=480
```

Alternatively, you can also modify a `.yml` file from the Getting Started tutorial to include your software component:
```yml
    myapp:
        container_name: myapp
        image: myapp.armhf
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--cid=112 --name=img.argb --width=640 --height=480"
```
