@echo off 
title= ---- Simu Controleur de feux ---- 
%homedrive% 
REM cd\ 
REM cd windows\system32 
color 0A 
cls 
echo  ****************************************************** 
echo  ************** Simu Controleur de feux ***************
echo  ****************************************************** 
echo  ************Par Gaetan ABESSOLO 17/04/2020 ***********
echo  ****************************************************** 
:début 
echo 1 : Lance le controleur de feux
echo 2 : Fermer l'ordinateur 
echo 3 : Fermer l'ordinateur et redemarrer 
echo 4 : Rien faire. 
echo 5 : Fermer la session.
set /p choix=Que voulez-vous faire? : 
( 
if not %choix%=='' set choix=%choix:~0,1% 
if %choix%==1 goto firectrl
if %choix%==2 goto shutdown 
if %choix%==3 goto shut_&_Restart 
if %choix%==4 goto end 
if %choix%==5 goto close_session  
) 
echo %choix% n'est pas bon ! 
goto début 
:close_session 
echo Vous avez choisi de fermer la session. 
shutdown -l -f -t 0 
goto end 
:shutdown 
echo Vous avez choisi de fermer l'ordinateur. 
shutdown -s -f -t 0 
goto end 
:shut_&_Restart 
echo Vous avez choisi de fermer et redémarrer l'ordinateur. 
shutdown -r -f -t 0 
goto end 
:firectrl 
echo Vous avez choisi de lancer le contrôleur de feux 
start /b FireControl.exe 
TIMEOUT /T 15
start /b appletviewer -J-Djava.security.policy=java.policy Applet.html
goto end 
:end