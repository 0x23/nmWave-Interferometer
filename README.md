# An Open Source Interferometer for Accurate Distance Measurements

The goal of this project is to provide plans and techniques for building an interferometer that can accurately measure changes in distance over a range of hundreds of millimeters with sub-micrometer precision. The objective is to use low-cost, widely available components and achieve a good balance between ease of manufacturing and performance.

This repository is currently work in progress...

![Interferometer](images/interferometer.jpg)

## Laser Safety

This project involves collimated laser beams with power levels above eye-safe limits able to cause permanent eye damage.
Please use proper precautions, do your own research, and respect laser safety guidelines.

<img src="images/laser_hazard.jpg" alt="Laser Hazard" width="7%"/>

[3D-Printed Grinding Tools](tools_3dprinted/beam_splitter_22mmx13mm)

## CAD Models

[NEW DESIGN - Version 5] FreeCAD models of the design are now online, take a look! 

<img src="images/FreeCAD-Model.jpg" alt="Laser Hazard" width="100%"/>

## Beam Splitter

The Beam splitter is made from regular float glass with a thickness of about 6 mm. It does not requre any coatings and uses the inherent reflectivity of glass surfaces.
It is ground to size using a diamond wetstone and a series of 3D printed jigs. The models for 3d printing can be found here:

## Laser Diodes

The current recommandation is the: $${\color{lightgreen}Osram \space PLT5 \space 516FA}$$ costing about 12$

For this project the laser source requires a long coherence length, which only certain laser diode models can provide.
Below is a table of laser diodes that I tested or want to test in future:
<br><br>

| Part Number           | Nominal Current | Optimal Drive Current | Power   | Wavelength  | Notes                                             | Plus on case |
|-----------------------|-----------------|-----------------------|---------|-------------|---------------------------------------------------|-----------|
| SLD3232VF             | 55mA            | ?                     | 50mW    | 405nm       | Not good                                          | Yes       |
| QL65E7SA              | 50mA            | 30mA                  | 7mW     | 650nm       | Good                                              | Yes       |
| QL65E7SB or C         | 50mA            | 30mA                  | 7mW     | 650nm       | Good                                              | Yes       |
| PLT520B               | 225mA           | ?                     | 110mW   | 520nm       | Very good (coherence length >4m)                  | Yes       |
| PLT5 516FA            | 100mA           | 36mA                  | 30mW    | 516nm       | (Very) good (CURRENT CHOICE)                      | No        |
| PLT5 520EB_Q          | -               | ?                     | 20mW    | 520nm       | Not tested (datasheet suggests very good)         | No        |
| PLT5 450(G)B          | 80mA            | 21mA                  | 100mW   | 450nm       | Okay                                              | No        |
| PLT5 520DB            | -               | ?                     | 10mW    | 520nm       | Not tested (probably not good)                    | No        |
| PLT5 518FB_P          | -               | ?                     | 30mW    | 518nm       | Not tested (probably not good)                    | No        |

## Photo Diodes

For the project fast photodiodes for the visible spectral range are required. Silicon PIN Photodiodes satisfy these requirements and are readily available.
To get a good signal a small active area of about 1mm² without a lens is prefereable. 

The current choice is the $${\color{lightgreen}SFH229}$$ photodiode in a 3mm LED casing costing about 0.70$.

It is sensitive in the range of 380 nm bis 1100 nm and has a switching time of 10ns. To to reduce the footprint and remove the lense a 3D printed jig and sandpaper can be used.
Mor information on this subject is coming soon..


