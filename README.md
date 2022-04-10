# Driller Data Extractor (Mirror)

Mirror of the Driller (and some other Freescape games) extractor.

The repository contains an extract of the originally provided archive (`drildump.zip`). The original README is [this](README).

The original website (https://www.seasip.demon.co.uk/ZX/Driller) is now offline; the last version can still be browsed via [Wayback Machine](https://web.archive.org/web/20200116141513/https://www.seasip.demon.co.uk/ZX/Driller).

## Useful information

The project compiles on recent compilers (e.g. GCC 9.4).

A copy of `driller.sna` can be downloaded from [here](https://k1.spdns.de/Vintage/Sinclair/Software/ZX%20Spectrum%20Software/Games/D/Driller%20(Incentive)/driller.sna).

The POV-Ray functionality appears not to work (fully) correctly; all (few) rooms I've tested rendered to blank or so images.

## Original website text

Copy of the original website text, for convenience.

### Driller room format

Nearly all of Driller is stored in a database that is pretty much machine- independent. On the Spectrum, the database is located at CF3Eh in memory. On the CPC, the database is 79ADh bytes from the beginning of a CPCEMU- format snapshot, which probably makes its address 78ADh. On the PC version, the database is 9B40h bytes from the start of DRILLE.EXE.

A lot of this document will make more sense if you've used the 3D Construction kit or read its manual.

### Driller data extractor

DRILDUMP.ZIP (50k) contains C source and a DOS EXE for a program to display the contents of the Driller, Dark Side and Total Eclipse databases, either in text or POV-Ray format.

### Database format

```
+0	DB	room_count
+1	DW	length_of_database
+3	DB	start_location_id
+4	DB	start_entrance		;ID of entrance within room
+5	DB
+6	DB	Initial jet energy
+7	DB	Initial jet shield
+8	DB	Initial probe energy
+9	DB	Initial probe shield
+0Ah	DS	3Ch	;Shade patterns
+46h	DW	offset	;Offset of table of ??? from database start
+48h	DW	offset	;Offset of table of global bytecode routines
			;from database start
...
+0B4h	DB	'hh:mm:ss'	;Time limit
+0BCh	DB
+0BDh	DB
+0BEh	DB	initial extend, 1 based (so 1 is Extend 0)
+0BFh	DB
+0C0h	DB
+0C1h	DB	start in probe (0) or jet (nonzero)
+0C2h	DB	initial step index   (0-7)
+0C3h	DB	initial angle index  (0-5)

At Database + 0C8h is a table of room definitions. Each entry is the offset from the start of the database of a room. The table can have up to 35 entries; spare ones are filled with zeroes.

In the supplied Driller, we have:

Location      ID  Address      Location            ID  Address
==============================================================
Amethyst       1  0xD0B2       Amethyst stores     19  0xE196
Lapis Lazuli   2  0xD3B1       Obsidian stores     20  0xE354
Emerald        3  0xD541       K2 Complex (Topaz)  21  0xE460
Malachite      4  0xE8BC       K1 Complex          22  0xE561
Ruby           5  0xE78E       K1/K2 Linker        23  0xE658
Aquamarine     6  0xEAA6       K3 Complex (Beryl)  25  0xF8A9
Beryl          7  0xEC63       Hangar              27  0xF68D
Topaz          8  0xD747       K4 Complex          28  0xFB92
Niccolite      9  0xEE3F       Unknown (Basalt)    31  0xE6D2
Alabaster     10  0xF044       Mitral              32  0xFA20
Opal          11  0xF21A       End screen         127  0xFA9E
Quartz        12  0xF37E
Diamond       13  0xF519
Obsidian      14  0xD995
Basalt        15  0xDBED
Ochre         16  0xDD6D
Graphite      17  0xDEB7
Trachyte      18  0xE01B
===============================================================

Format of a room is:

+0	DB	Meaning unknown.
+1	DB	number of objects in the room
+2	DB	room ID number
+3	DW	offset from start of room to room bytecode routines
+5	DB	scale		;Room scale; 1 is normal for outside areas.
				;If you set it to 15 you get a bizarre
				;room where gas rigs come up to your waist.
;
; I'm calling the next 4 bytes "Palette 0" to "Palette 3". On the Spectrum
; their meanings are as given below, but on the CPC and PC they are
; selections of which inks to use from the palette.
;
+6	DB	border		;[palette 0] Normal border colour
+7	DB	sensbdr		;[palette 1] Border colour when under fire
+8	DB	paper		;[palette 2] Background colour of room
				;		(+8 if BRIGHT)
+9	DB	ink		;[palette 3] Foreground colour of room
+10	DB	x,z		;Coordinates of the gas pocket
+12	DB	diameter	;Size of the gas pocket. If this is small,
				;you have to put the rig right on the centre.
+13	DB	room_name	;12 bytes
+25	objects

General object format is:
+0	DB	type		;Bits 0-4 = type
				;Bit 5 set: Object destroyed
				;Bit 6 set: Object invisible
				;Bit 7 set: Object is initially invisible
+1	DB	x,y,z		;X,Y,Z coordinates of bounding cube.
+4	DB	dx,dy,dz	;Dimensions of bounding cube
+7	DB	id		;Object ID
+8	DB	len		;Length of this object record.
	... additional data ...
;
; Object types are:
;
; 0. Entrance. No additional data. The "dx,dy,dz" fields are used for
;   rotation about the X, Y and Z axes. 1 unit in these fields = 5 degrees.
;
; 1. Cube. The shape is given by the bounding cube.
;         Additional data are three bytes of textures - 6 nibbles for the
;         6 surfaces.
;
; 2.Sensor
;   The bounding cube size is always 0.
;   There are 4 bytes of additional data, presumably recording range and
;   firing interval.
;
; 3. Rectangle
;
;   There is 1 byte of additional data, containing the textures of the two
;   surfaces.
;
; 4. Pyramid, point faces east  (+X)
; 5. Pyramid, point faces west  (-X)
; 6. Pyramid, point faces up    (+Y)
; 7. Pyramid, point faces down  (-Y)
; 8. Pyramid, point faces north (+Z)
; 9. Pyramid, point faces south (-Z)
;
; There are 7 bytes of additional data: three texture bytes, and then two
; pairs of slope coordinates (s1,s2) and (s3, s4). These are the two opposite
; corners of the narrow surface of the pyramid, relative to the pyramid origin.
; If they are the same, then the pyramid comes to a point. If they are
; different, the pyramid can have a flat top (cf the shed in Amethyst) or a
; wedge shape (cf the "nose" in Malachite).
;
; 10. Line
;
; There are 7 bytes of additional information:
; One texture byte, and then two sets of (x,y,z) coordinates for the start
; and end of the line.
;
; 11. Triangle.      As line, but three sets of coordinates.
; 12. Quadrilateral. As line, but four sets of coordinates.
; 13. Pentagon.      As line, but five sets of coordinates.
; 14. Hexagon.       As line, but six sets of coordinates.
;
; The objects can be longer than these minimum lengths. If so, the remaining
; bytes form bytecode instructions. The opcodes are:
;
; bit 7 set if opcode is executed when object shot.
;       clear if opcode is executed when object touched.
;

1	+SCORE  a b c		;Add value to the score
2	+ENERGY	n		;Add value to energy
3	TOGVIS	obj		;Toggle visibility of object here
4	VIS	obj		;Show object here
5	INVIS	obj		;Hide object here
6	RTOGVIS room obj	;Toggle visibility of object elsewhere
7	RVIS    room obj	;Show object elsewhere
8	RINVIS  room obj	;Hide object elsewhere
9	INCR	var		;Increment variable
10	DECR	var		;Decrement variable
11	IFEQ	var value	;Only continue if variable matches value
12	SETBIT	bit		;Set a bit flag
13	CLRBIT	bit		;Clear a bit flag
14	IFBIT	bit value	;Only continue if bit matches value
15	SOUND	n		;Play sound
16	DESTROY obj		;Destroy object here
17	RDESTR  obj		;Destroy object elsewhere
18	GOTO	room entrance
19	+SHIELD n		;Increase/decrease shield
20	SETV	var value	;Assign value to variable
21	SWAPJET			;Switch vehicle
22-24				;Unused?
25	SPFX	n		;Used to flash the border or change the
				;room colours. SPFX works like this:
				;	SPFX xyh: Set palette (x) to palette
				;		  (y). For example,
				;	SPFX 31h sets room ink (palette 3) to
				;       border-under-fire (palette 1).
26	REDRAW			;Redraw window
27	DELAY	n		;Wait n/50 sec.
28	SYNCSND	n		;Play synchronised sound
29	TOGBIT	n		;Toggle bit flag
30	IFVIS   obj		;Only continue if object visible
31	IFINVIS	obj		;Only continue if object invisible
;
; The room also has its own bytecode collection, at the offset given
; in offsets 3,4.
;
; Format is:
;	DB	n_subroutines
;	 DB	bytes_in_subroutine	; for each
;	 DB	bytecode		; subroutine
```

### Font

The font is just before the main database - in the Spectrum version, it's at CDDCh. It has 59 characters, each 6 rows high.

### Global objects

The global objects (the gas rig, and the skanner) are not stored in the same place as the rest of the data. In the Spectrum version, they live at 87A5h.
