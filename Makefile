sw:
	gcc -o share_write share_write.c
sv:
	gcc -o share_view share_view.c
sc:
	gcc -o share_clean share_clean.c
clean:
	rm -rf share_clean share_write share_view