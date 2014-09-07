Lp3 :: Input
============

Abstracts controller and device input for use with games. The goal is to
provide access to any possible control, while giving games an interface that
offers a clean view of a logical controller.

The root of it all is "DeviceSource", which is a factory for Device objects.
A device is a collection of digital or analog buttons which return boolean
values or float values from 0.0 to 1.0.

From this, a game can create a platform independent means of control.




App Layer
---------
This system layer gives access to any controls. From here it is possible to
tell what controls are connected, their names, and the names of the buttons.

ControlDevice - defines a device that can be used. Contains a list of keys,
of size GetKeyCount(). It is possible to find a name of every key, and also
to find what key is pressed for any control (this enables users to hit a button
in order to select a key). It can also get the current key state of any key.

DeviceFactory - This is a source for devices. For example, the DirectInput
and XInput API are both used, and present this interface. These contain all
such devices, and so can be used to get the count of devices, the device names,
and the key state of each device.



Demos
-----

* src/Lp3/Input/DirectInput/DxDemo.mcpp
    - Shows off how to use the Direct Input code directly, making it easy to
      perform tests in real time.
      Requires the Squirrel Debugger to be turned on so I could avoid Win32
      GUI stuff.

* src/Lp3/Input/Dreamcast/DreamcastDemo.mcpp
    - Shows off the Dreamcast controller code. Prints to the status of
      controls to the TV screen directly.


