%%Programa de comunicación serial por: Alberto González Moreno
clc
clear all 
close all
txt = "hola";
%Restea todos los puertos
instrreset
A = input("A qué número de puerto COM te deseas conectar? R: ");
B = input("Inserta la velocidad R: ");
s = serial(['COM' num2str(A)],'BaudRate',B);
s.Terminator = 0;
%Indica el numero de bytes%
%s.BytesAvailableFcnCount = 8;
%Abre el puerto serial 
fopen(s);
while txt ~= "salir"
prompt = "Que color desea modificar? (r = Rojo, g = Verde, b = Azul): ";
txt = input(prompt,"s");
    prompt = "Inserte la intensidad (0-255):";
    number = [input(prompt,"s") ','];
    fprintf(s,txt);
    fprintf(s,number);
        C = fscanf(s,"%s");
        fprintf("La tiva dice: %s \n",C);
end
fclose(s);