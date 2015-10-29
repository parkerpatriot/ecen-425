$(LAB)final.bin:	$(LAB)final.s
		nasm $(LAB)final.s -o $(LAB).bin -l $(LAB).lst

$(LAB)final.s:	clib.s isr.s yaks.s yakc.s $(LAB)_app.s
		cat clib.s isr.s yaks.s yakc.s $(LAB)_app.s > $(LAB)final.s

yakc.s:		yakc.c yakk.h yaku.h clib.h
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

$(LAB)_app.s:	$(LAB)_app.c yakk.h yaku.h clib.h
		cpp $(LAB)_app.c $(LAB)_app.i
		c86 -g $(LAB)_app.i $(LAB)_app.s			

clean:
		rm *.bin *.lst *final.s yakc.s *_app.s yakc.i *_app.i

.PHONY: export
export:
	export PATH=$$PATH:/ee2/ee525/bin

