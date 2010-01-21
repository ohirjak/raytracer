status:

- ray-tracer (s nastavitelnou max. hlbkou rekurzie)
	- single-threaded implementacia
	- multi-threaded implementacia
- scena so statickymi objektami (gule, svetla) a statickou kamerou
- vystup do .bmp


TODO:

- pouzit 16x16 strukturu pamate (blocklinear)
- nacitanie sceny zo suboru (objekty, kamera, pohlad, prip. 3D grid pre CT a pod.)
- implementovat na gpu (opencl/cuda), grid
- zostavit "zakladnu scena" vizualizacie: Cornell Box
- pouzit Stanford Bunny ako volumetricky model

- Refraction
- Photon Mapping
- Monte Carlo Ray Tracing
	- Indirect Illumination
	- Glossy Reflection
	- ...
- Documentation (Doxygen)
- BRDF
- Shadow rays
	- Soft Shadows
- KD-tree


ostatne moznosti - ray casting oktalneho stromu alebo volumetrickeho modelu, napr. 16x16x16 voxelov s kvadrom 3x2x4 voxely (nepriehladne)

