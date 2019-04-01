## Processing image data with OpenCV and controlling Kiwi using Python

This Python-template demonstrates how to develop a software module to process video data (eg., image detection, etc.) and to control Kiwi.

Prerequisites:
* [You need to install Docker for your platform](https://docs.docker.com/install/linux/docker-ce/debian/#install-docker-ce)
* [You need to install `docker-compose`](https://docs.docker.com/compose/install/#install-compose)
* You have successfully completed the "Getting Started" tutorials [here](https://github.com/chalmers-revere/opendlv-tutorial-kiwi/tree/master).
* You have a recording file (`.rec`) with some video frames.
* You need to install `libcluon` (example below is for Ubuntu 18.04 LTS):
```Bash
sudo add-apt-repository ppa:chrberger/libcluon
sudo apt-get update
sudo apt-get install libcluon
```
* You need to install Python, make, protobuf, and OpenCV (example below is for Ubuntu 18.04 LTS):
```Bash
sudo apt-get install --no-install-recommends \
    build-essential \
    python-protobuf \
    python-sysv-ipc \
    python-numpy \
    python-opencv \
    protobuf-compiler
```

---

### Developing and testing the Python application on your laptop using replay mode

This template folder contains an example how to use Python to process data residing in a shared memory area using OpenCV for image processing.

* Step 1: Assuming that you have a folder `~/recordings`, where you have at least one `.rec` file from your experiments with Kiwi. Now, you can start the h264 decoder and webapp for replaying as follows (the actual h264 decoder is built once during the first call):
```bash
docker-compose -f h264-decoder-viewer.yml up
```

* Step 2: Clone this repository:
```bash
cd $HOME
git clone https://github.com/chalmers-revere/opendlv-tutorial-kiwi.git
cd opendlv-tutorial-kiwi/image-postprocessing-opencv-python
```

* Step 3: The Python script uses messages from the OpenDLV Standard Message Set. To use them with Python, just run:
```bash
make
```
This step needs to be repeated whenever you change something in the message specifications.


Next, start a webbrowser and connect to your local webapp: http://localhost:8081 and open the folder view. Select one of the `.rec` files for replay and begin the replay to fill the shared memory with some image data; you can pause the replay shortly after you saw the image in your browser.

* Step 4: Run the Pyton module from the folder `image-postprocessing-opencv-python`:
```bash
python myApplication.py
```

The application should start and wait for images to come in. Now, continue to replay your recording; the Python application should open a new window and display the frames. Furthermore, the code also display the distance readings from the sensors.

You can stop the Pythong application by pressing `Ctrl-C`. When you are modifying the Python application, repeat step 4 after any change modification.

---

### Deploying and testing the Python application on Kiwi

After you have completed your development locally and you are ready for testing on Kiwi, you need to package the software into a Docker image.

* Step 1: Have the previous tutorial completed.

* Step 2: **Make sure that you commented all debug windows from OpenCV as there won't be a GUI available on Kiwi.''

* Step 3: Don't forget to change the CID from 253 (used for replay mode) to 112 (used on Kiwi) so that the messages that you are sending are processed properly.

* Step 4: Build the Docker image (don't forget the trailing `.`). The first time build process can take a while:
```Bash
docker build -t myapp -f Dockerfile.armhf .
```

* Step 5: Next, save the created Docker image to a file:
```Bash
docker save myapp > myapp.tar
```

* Step 6: Now, you copy the Docker image to Kiwi (the upload can take a few minutes):
```Bash
scp -P 2200 myapp.tar pi@192.168.8.1:~
```

* Step 7: Your application image is now on Kiwi; next, login to Kiwi to start it:
```Bash
ssh -p 2200 pi@192.168.8.1
```

* Step 8: Assuming that the Getting Started Tutorial 2 (Controlling Kiwi using your webbrowser) is still running, you can load your application into Docker and start it:
```Bash
docker load < myapp.tar
docker run --rm -ti --init --ipc=host --net=host -v /tmp:/tmp myapp:latest 
```

When everything has worked correctly, you should see a message of type opendlv.proxy.AngleReading in the message overview of the web application.

---

### Updating your Python application on Kiwi

After you have completed your development locally and you are ready for testing on Kiwi, you need to package the software into a Docker image.

* Step 1: Have the previous tutorial completed.

* Step 2: **Make sure that you commented all debug windows from OpenCV as there won't be a GUI available on Kiwi.''

* Step 3: Don't forget to change the CID from 253 (used for replay mode) to 112 (used on Kiwi) so that the messages that you are sending are processed properly.

* Step 4: Copy your application myApplication.py to Kiwi:
```Bash
scp -P 2200 myApplication.py pi@192.168.8.1:~
```

* Step 5: Your application image is now on Kiwi; next, login to Kiwi to start it:
```Bash
ssh -p 2200 pi@192.168.8.1
```

* Step 6: Assuming that the Getting Started Tutorial 2 (Controlling Kiwi using your webbrowser) is still running, you can load your application into Docker and start it:
```Bash
docker run --rm -ti --init --ipc=host --net=host -v /tmp:/tmp -v $HOME/myApplication.py:/opt/sources/myApplication.py myapp:latest
```
The parameter `-v $HOME/myApplication.py:/opt/sources/myApplication.py` is mapping your Python application into the running Docker container and thus, replacing the one that is contained in the Docker image. If you need more files to run your application (e.g., `.xml` files), simply add further parameters like `-v $HOME/myFile.xml:/opt/sources/myFile.xml` before the past part of the previous command `myapp:latest`. 


