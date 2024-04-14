## A modular, open-source platform for sub-MHz NMR

#### Introduction
Many students, researchers and hobbyists will be familiar with the [Arduino](https://arduino.cc) open-source-electronics ecosystem, which provides an extraordinarily simple way to interface sensors (or other input devices) and actuators (output) with logic programs, e.g. C code, to create a wide variety of standalone control devices termed embedded systems.  A Nuclear Magnetic Resonance (**NMR**) spectrometer can be regarded as one specific type of embedded system: the output is a magnetic field produced by a coil, the input is a magnetic field (detected and recorded by a digitizer), and a pulse programmer keeps timing and data in order.

The **NMRduino** is a magnetic resonance spectrometer based on (but we must stress, not endorsed or supported by) Arduino that we have developed over recent years to study hyperpolarized NMR systems, NMR relaxation, high-resolution spectroscopy, and coherent control at low magnetic fields, as well as teach basic principles of magnetic resonance to student beginners

Main features are:

-  Compact, plug-and-play hardware.  A credit-card-sized circuit board contains all electronic components and connects to any laptop, desktop or Raspberry Pi computer via USB.  Includes pulse programmer and analog sampling up to 100 kHz.

-  Transparent, intuitive control interface.  User-specific pulse sequences (1 μs time resolution) can be written to control both DC and AC magnetic fields up to several hundred kHz.  Open access to low-level programming interface for advanced users.

-  Flexibility.  Can be connected to conventional rf-inductive pickup coils, or alternative sensors such as atomic magnetometers.

#### External resources
Open Source Imaging (FOSS/OSHW magnetic resonance directory): https://www.opensourceimaging.org/project/nmrduino/ </br>
Journal of Magnetic Resonance (article by the authors of NMRduino, May 2024): https://doi.org/10.1016/j.jmr.2024.107665
