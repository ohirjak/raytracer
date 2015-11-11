status:

- ray-tracer (s nastavitelnou max. hlbkou rekurzie)
	- single-threaded implementacia
	- multi-threaded implementacia
- scena so statickymi objektami (gule, svetla) a statickou kamerou
- vystup do .bmp


TODO:

- ray caster
- pridat cut plane, poziciu kamery a zobrazenie pozicie svetla
- pouzit 16x16 strukturu pamate (blocklinear) - iba pre gpu?
- nacitanie sceny zo suboru (volume data, kamera, pohlad, prip. 3D grid pre CT a pod.)
- implementovat na gpu (opencl/cuda), grid
- zostavit "zakladnu scena" vizualizacie: Cornell Box
- pouzit Stanford Bunny ako volumetricky model
- pouzit standardny rasterizer renderer na urcenie visibility a hit-ov?
- spojit s inymi oblastami, ktore potrebuju vypocet occlusion a pod. (sound?)
- "ina" matematika, ktora by riesila tieto problemy by default

- Refraction
- Photon Mapping
- Monte Carlo Ray Tracing/Casting
	- Indirect Illumination
	- Glossy Reflection
	- ...
- Documentation (Doxygen)
- BRDF
- Shadow rays
	- Soft Shadows
- KD-tree, oct-tree, aabb

ostatne moznosti - ray casting oktalneho stromu alebo volumetrickeho modelu, napr. 16x16x16 voxelov s kvadrom 3x2x4 voxely (nepriehladne)
