# rpi altair 
raspberry pi ros workspace
### initial setup:
- clone the repo, it's  a whole workspace. 

- ```cd erc_ws```
- ```catkin_make```

## Usage:

- First connect the crossfire connected arduino nano.
- Then connect the Drive arduino due.
- Then start roscore in the master computer.

- Run in the master computer:
- ```cd erc_ws```
- ```source devel/setup.bash```
- ```roslaunch ros_handler navigation.launch```
