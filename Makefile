OBJS := main.o cmd_ls.o cmd_cp.o cmd_handle.o #依赖文件
TARGET := tinyshell #目标

$(TARGET):$(OBJS)
	gcc $^ -o $@ # $^依赖的所有文件 $@ 目标文件
	echo "Done."
#main.o:main.c
#	@gcc -c main.c -o main.o
#cmd_ls.o:cmd_ls.c
#	@gcc -c cmd_ls.c -o cmd_ls.o

%.o:%.c
	gcc -c $< -o $@  #$< 依赖的第一个文件 $@ 目标文件

clean:
	rm *.o $(TARGET)
	echo "Clean."