PICTURES = amethyst.png lapis.png emerald.png malachite.png ruby.png \
           aquamarine.png beryl.png topaz.png obsidian.png ochre.png \
	   graphite.png trachyte.png stores1.png stores2.png linker.png \
	   k4complex.png k3complex.png k2complex.png k1complex.png \
	   niccolite.png alabaster.png opal.png quartz.png diamond.png \
	   hangar.png mitral.png end.png unknown.png basalt.png \
	   \
	   darkside.png thethys.png psyche.png pollux.png nereicl.png \
	   ganymede.png iapetus.png sirius.png procyon.png fomalhaut.png \
	   umbriel.png titania.png oberon.png triton.png regulus.png \
	   monoceros.png antares.png lightside.png gstore.png cstore.png \
           canopus.png centauri.png crux.png eqtunnel.png dtunnel.png \
	   ltunnel.png io.png

ZIP = COPYING Makefile README alabaster.hdr amethyst.hdr aquamarine.hdr \
	basalt.hdr beryl.hdr diamond.hdr emerald.hdr end.hdr graphite.hdr \
	hangar.hdr k1complex.hdr k2complex.hdr k3complex.hdr k4complex.hdr \
	lapis.hdr linker.hdr malachite.hdr mitral.hdr niccolite.hdr \
	obsidian.hdr ochre.hdr opal.hdr quartz.hdr ruby.hdr stores1.hdr \
	stores2.hdr topaz.hdr trachyte.hdr unknown.hdr \
	\
	darkside.hdr thethys.hdr psyche.hdr pollux.hdr nereicl.hdr \
	ganymede.hdr iapetus.hdr sirius.hdr procyon.hdr fomalhaut.hdr \
	umbriel.hdr titania.hdr oberon.hdr triton.hdr regulus.hdr \
	monoceros.hdr antares.hdr lightside.hdr gstore.hdr cstore.hdr \
        canopus.hdr centauri.hdr crux.hdr eqtunnel.hdr dtunnel.hdr \
	ltunnel.hdr io.hdr \
	\
	drildump.c drildump.exe driller.txt 

CFLAGS = -g -Wall

all:	drildump 

%.png:	%.pov
	x-povray $(POVRAYFLAGS) -I$<

pictures:	$(PICTURES)

amethyst.pov:	drildump amethyst.hdr
	cp -f amethyst.hdr $@
	./drildump -pov >>$@

lapis.pov:	drildump lapis.hdr
	cp -f lapis.hdr $@
	./drildump -room 1 -pov >>$@

emerald.pov:	drildump emerald.hdr
	cp -f emerald.hdr $@
	./drildump -room 2 -pov >>$@

topaz.pov:	drildump topaz.hdr
	cp -f topaz.hdr $@
	./drildump -room 3 -pov >>$@

obsidian.pov:	drildump obsidian.hdr
	cp -f obsidian.hdr $@
	./drildump -room 4 -pov >>$@

basalt.pov:	drildump basalt.hdr
	cp -f basalt.hdr $@
	./drildump -room 5 -pov >>$@

ochre.pov:	drildump ochre.hdr
	cp -f ochre.hdr $@
	./drildump -room 6 -pov >>$@

graphite.pov:	drildump graphite.hdr
	cp -f graphite.hdr $@
	./drildump -room 7 -pov >>$@

trachyte.pov:	drildump trachyte.hdr
	cp -f trachyte.hdr $@
	./drildump -room 8 -pov >>$@

stores1.pov:	drildump stores1.hdr
	cp -f stores1.hdr $@
	./drildump -room 9 -pov >>$@

stores2.pov:	drildump stores2.hdr
	cp -f stores2.hdr $@
	./drildump -room 10 -pov >>$@

k2complex.pov:	drildump k2complex.hdr
	cp -f k2complex.hdr $@
	./drildump -room 11 -pov >>$@

k1complex.pov:	drildump k1complex.hdr
	cp -f k1complex.hdr $@
	./drildump -room 12 -pov >>$@

linker.pov:	drildump linker.hdr
	cp -f linker.hdr $@
	./drildump -room 13 -pov >>$@
#
# "Unknown" means it's the area entitled "Unknown area", not that I 
# don't know what area it is!
#
unknown.pov:	drildump unknown.hdr
	cp -f unknown.hdr $@
	./drildump -room 14 -pov >>$@

ruby.pov:	drildump ruby.hdr
	cp -f ruby.hdr $@
	./drildump -room 15 -pov >>$@

malachite.pov:	drildump malachite.hdr
	cp -f malachite.hdr $@
	./drildump -room 16 -pov >>$@

aquamarine.pov:	drildump aquamarine.hdr
	cp -f aquamarine.hdr $@
	./drildump -room 17 -pov >>$@

beryl.pov:	drildump beryl.hdr
	cp -f beryl.hdr $@
	./drildump -room 18 -pov >>$@

niccolite.pov:	drildump niccolite.hdr
	cp -f niccolite.hdr $@
	./drildump -room 19 -pov >>$@

alabaster.pov:	drildump alabaster.hdr
	cp -f alabaster.hdr $@
	./drildump -room 20 -pov >>$@

opal.pov:	drildump opal.hdr
	cp -f opal.hdr $@
	./drildump -room 21 -pov >>$@

quartz.pov:	drildump quartz.hdr
	cp -f quartz.hdr $@
	./drildump -room 22 -pov >>$@

diamond.pov:	drildump diamond.hdr
	cp -f diamond.hdr $@
	./drildump -room 23 -pov >>$@

hangar.pov:	drildump hangar.hdr
	cp -f hangar.hdr $@
	./drildump -room 24 -pov >>$@

k3complex.pov:	drildump k3complex.hdr
	cp -f k3complex.hdr $@
	./drildump -room 25 -pov >>$@

mitral.pov:	drildump mitral.hdr
	cp -f mitral.hdr $@
	./drildump -room 26 -pov >>$@

end.pov:	drildump end.hdr
	cp -f end.hdr $@
	./drildump -room 27 -pov >>$@

k4complex.pov:	drildump k4complex.hdr
	cp -f k4complex.hdr $@
	./drildump -room 28 -pov >>$@
#
# Pictures from The Dark Side
#
darkside.pov:	drildump darkside.hdr
	cp -f darkside.hdr $@
	./drildump darkside.sna -dark -pov >>$@

thethys.pov:	drildump thethys.hdr
	cp -f thethys.hdr $@
	./drildump darkside.sna -room 1 -dark -pov >>$@

psyche.pov:	drildump psyche.hdr
	cp -f psyche.hdr $@
	./drildump darkside.sna -room 2 -dark -pov >>$@

pollux.pov:	drildump pollux.hdr
	cp -f pollux.hdr $@
	./drildump darkside.sna -room 3 -dark -pov >>$@

nereicl.pov:	drildump nereicl.hdr
	cp -f nereicl.hdr $@
	./drildump darkside.sna -room 4 -dark -pov >>$@

ganymede.pov:	drildump ganymede.hdr
	cp -f ganymede.hdr $@
	./drildump darkside.sna -room 5 -dark -pov >>$@

iapetus.pov:	drildump iapetus.hdr
	cp -f iapetus.hdr $@
	./drildump darkside.sna -room 6 -dark -pov >>$@

sirius.pov:	drildump sirius.hdr
	cp -f sirius.hdr $@
	./drildump darkside.sna -room 7 -dark -pov >>$@

procyon.pov:	drildump procyon.hdr
	cp -f procyon.hdr $@
	./drildump darkside.sna -room 8 -dark -pov >>$@

fomalhaut.pov:	drildump fomalhaut.hdr
	cp -f fomalhaut.hdr $@
	./drildump darkside.sna -room 9 -dark -pov >>$@

umbriel.pov:	drildump umbriel.hdr
	cp -f umbriel.hdr $@
	./drildump darkside.sna -room 10 -dark -pov >>$@

titania.pov:	drildump titania.hdr
	cp -f titania.hdr $@
	./drildump darkside.sna -room 11 -dark -pov >>$@

oberon.pov:	drildump oberon.hdr
	cp -f oberon.hdr $@
	./drildump darkside.sna -room 12 -dark -pov >>$@

triton.pov:	drildump triton.hdr
	cp -f triton.hdr $@
	./drildump darkside.sna -room 13 -dark -pov >>$@

regulus.pov:	drildump regulus.hdr
	cp -f regulus.hdr $@
	./drildump darkside.sna -room 14 -dark -pov >>$@

monoceros.pov:	drildump monoceros.hdr
	cp -f monoceros.hdr $@
	./drildump darkside.sna -room 15 -dark -pov >>$@

antares.pov:	drildump antares.hdr
	cp -f antares.hdr $@
	./drildump darkside.sna -room 16 -dark -pov >>$@

lightside.pov:	drildump lightside.hdr
	cp -f lightside.hdr $@
	./drildump darkside.sna -room 17 -dark -pov >>$@

gstore.pov:	drildump gstore.hdr
	cp -f gstore.hdr $@
	./drildump darkside.sna -room 18 -dark -pov >>$@

cstore.pov:	drildump cstore.hdr
	cp -f cstore.hdr $@
	./drildump darkside.sna -room 19 -dark -pov >>$@

canopus.pov:	drildump canopus.hdr
	cp -f canopus.hdr $@
	./drildump darkside.sna -room 20 -dark -pov >>$@

centauri.pov:	drildump centauri.hdr
	cp -f centauri.hdr $@
	./drildump darkside.sna -room 21 -dark -pov >>$@

crux.pov:	drildump crux.hdr
	cp -f crux.hdr $@
	./drildump darkside.sna -room 22 -dark -pov >>$@

eqtunnel.pov:	drildump eqtunnel.hdr
	cp -f eqtunnel.hdr $@
	./drildump darkside.sna -room 23 -dark -pov >>$@

dtunnel.pov:	drildump dtunnel.hdr
	cp -f dtunnel.hdr $@
	./drildump darkside.sna -room 24 -dark -pov >>$@

ltunnel.pov:	drildump ltunnel.hdr
	cp -f ltunnel.hdr $@
	./drildump darkside.sna -room 25 -dark -pov >>$@

io.pov:	drildump io.hdr
	cp -f io.hdr $@
	./drildump darkside.sna -room 26 -dark -pov >>$@

clean:
	rm -f *.png
	rm -f *.pov

zip:	$(ZIP)
	zip drildump.zip $(ZIP)
