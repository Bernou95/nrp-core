xhost +
IMAGENAME=opensim:v1

sudo docker run \
  --rm \
  -it \
  --gpus all \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -e DISPLAY=$DISPLAY \
  -e NVIDIA_DRIVER_CAPABILITIES=compute,utility,graphics\
  -e QT_X11_NO_MITSHM=1 \
  $IMAGENAME