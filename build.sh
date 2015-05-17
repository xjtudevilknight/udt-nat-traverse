gcc -o server server.cpp -I ../udt-git/udt4/src/ -L ../udt-git/udt4/src  -ludt -lstdc++ -lpthread
gcc -o client client.cpp -I ../udt-git/udt4/src/ -L ../udt-git/udt4/src  -ludt -lstdc++ -lpthread
