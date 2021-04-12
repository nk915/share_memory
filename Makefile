sw:
	gcc -o share_write share_write.c -pthread
sv:
	gcc -o share_view share_view.c -pthread
sc:
	gcc -o share_clean share_clean.c -pthread
clean:
	rm -rf share_clean share_write share_view