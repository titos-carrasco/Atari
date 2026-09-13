# Netcat

Un experimento para comprender como programar para Fujinet utilizando la librería **fujinet-lib**

Se debe crear un directorio llamado Disk (**Netcat/Disk**) con:
* dos.sys
* autorun.sys

obtenidos desde el disco [n-handler.atr](https://apps.irata.online/Atari_8-bit/n-handler.atr):

```bash
$ mkdir Disk

$ atr fujinet-dos/n-handler.atr ls -a
autorun.sys  dos.sys      ncopy.com    nmkdir.com   nrmdir.com   whois.bas
config.com   dumbterm.bas ndel.com     npwd.com     ntrans.com
copy.com     ncd.com      ndir.com     nren.com     tictacto.bas

$ atr n-handler.atr get autorun.sys Disk/autorun.sys

$ atr n-handler.atr get dos.sys Disk/dos.sys

$ ls Disk
autorun.sys  dos.sys
```

Observaciones:
* fujinet-dos/ es la ruta en donde está n-handler.atr
* Para manipular los archivos **ATR** se utiliza **atr** que puede ser obtenida desde el proyecto [atari-tools](https://github.com/jhallen/atari-tools.git)
* Para generar el ATR final se utiliza **dir2atr** que puede ser obtenida desde el proyecto [AtariSIO](https://github.com/HiassofT/AtariSIO.git)
* Para las pruebas en Linux Debian se utiliza el emulador [Fujisan](https://github.com/pedgarcia/fujisan) que viene con soporte de **Fujinet**
* La máquina de destino está fija en el código (archivo **main.c**)

