%%Programa de comunicación serial por: Alberto González Moreno
clc
clear all 
close all
txt = "m";
%Restea todos los puertos
instrreset
s = serial(['COM7','BaudRate',115200);
s.Terminator = 0;
%Indica el numero de bytes%
%s.BytesAvailableFcnCount = 8;
%Abre el puerto serial 
fopen(s);
while txt ~= "salir"
prompt = "Que color desea modificar? (r = Rojo, g = Verde, b = Azul): ";
txt = input(prompt,"s");
    fprintf(s,txt);
    prompt = "Inserte la intensidad (0-255):";
    number = [input(prompt,"s") ','];
    fprintf(s,number);
end
fclose(s);