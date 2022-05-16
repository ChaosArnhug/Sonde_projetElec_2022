# Projet d'électonique 2022
Ce répo regroupe le code, les schémas Eagle et le Prothéus du projet d'électonique à l'Ephec en 2022 pour les 2ièmes années.

## Contexte 

* Les PIC’s deviennent avec le temps des composants universels des systèmes électroniques et des systèmes embarqués. Il semble donc opportun que les étudiants soient aptes à définir et développer une carte comportant non seulement un microcontrôleur PIC mais aussi les composants annexes minimaux nécessaires au bon fonctionnement de ce dernier

*  De manière à pouvoir ensuite tester et vérifier le bon fonctionnement de la carte PIC, il semble opportun de développer une application électronique se basant, au moinspour certaines de ses fonctions, sur le PIC

* Cette application sera un télémètre à ultrasons qui envoie auPIC la mesure d’une distance en cm

* La carte doit afficher la distance sur 2 afficheurs 7-segments

* La carte devra signaler une alerte lorsque la distance dépasse (ou tombe en dessous d’) un seuil critique défini par l’utilisateur

* Enfin, pour utiliser les connaissances du cours de Développement informatique II, le PIC devra communiquer avec une application PYTHON qui affichera la distance
mesurée et permettra de définir le seuil de déclenchement d’alerte

## Spécifications

* PIC : 18F458

* Sonde : HC-SR04

* Affichage distance : 2 7-segments à cathodes communes 

* Affichage alerte : LED rouge qui clignote

* Affichage aucune alerte : LED verte

* Gestion du seuil : interface codée en Python
