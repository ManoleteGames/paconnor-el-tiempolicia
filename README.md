
# PACONNOR, El tiempolicia

> Un shooter de acción y aventura para MS-DOS, ambientado en un futuro distópico.
> 
> Pude descargarse el último release y utilizarlo en cualquier emulador como DOSBOX, PCem, 86Box, etc..
> 
> O jugar directamente en [Itch.io](https://manoletegames.itch.io/paconnor-el-tiempolicia)

![MS-DOS](https://img.shields.io/badge/Platform-MS--DOS-555555)
![Language](https://img.shields.io/badge/Language-C-blue)
![Status](https://img.shields.io/badge/Status-In%20Development-orange)

---

## 📖 Índice

* [🎮 Descripción](#-descripción)
* [✨ Características](#-características)
* [🕹️ Controles](#️-controles)
* [🖥️ Requisitos](#️-requisitos)
* [🛠️ Compilación](#️-compilación)
* [📁 Estructura del proyecto](#-estructura-del-proyecto)
* [📸 Capturas](#-capturas)
* [📜 Historia](#-historia)
* [📋 Estado del desarrollo](#-estado-del-desarrollo)
* [👨‍💻 Autor](#-autor)

---

## 🎮 Descripción

**PACONNOR, El tiempolicia** es un videojuego de acción para MS-DOS inspirado en juegos clásicos como The chaos engine, True Lies y Demolition man.
El jugador controla a un hastiado inspector de policía que debe salvar el mundo liberándolo de una secta que lo controla todo.
El proyecto está desarrollado en **C con DJGPP**, utilizando gráficos VGA de 320×200 píxeles.

---

## ✨ Características

* 🎯 Acción con teclado y ratón.
* 🕹️ Perspectiva cenital (*top-down*).
* 🖥️ Gráficos VGA de 320×200.
* 💾 Compatible con MS-DOS.
* 🔫 Armas, enemigos y proyectiles.
* 🤖 Ciudad futurista y decadente.
* ⏳ Viajes en el tiempo.
* 🎵 Música y efectos de sonido retro (PC Speaker)

---

## 🕹️ Controles

| Acción         | Tecla               |
| -------------- | ------------------- |
| Moverse        | W S A D             |
| Esquivar       | SPACE               |
| Apuntar        | 🖱️ Ratón           |
| Disparar       | 🖱️ Botón izquierdo |
| Lanzar granada | 🖱️ Botón derecho   |
| Recargar arma  | R                   |
| Puños          | 1                   |
| Pistola        | 2                   |
| Escopeta       | 3                   |
| UZI            | 4                   |
| Rifle          | 5                   |
| Pausa          | P                   |
| Salir          | ESC                 |

---

## 🖥️ Requisitos

* PC compatible con MS-DOS.
* Procesador 486-DX2 66MHz o superior.
* Tarjeta gráfica VGA.
* Ratón compatible con MS-DOS.
* PC Speaker.

---

## 🛠️ Compilación

El proyecto utiliza **DJGPP** para generar el ejecutable DOS.

---

## 📁 Estructura del proyecto

```text
PACONNOR/
├── ASSETS/             # Recursos del juego
├───── BINARIES/        # Pantallas en modo texto
├───── FONTS/           # Fuentes de texto
├───── IMAGES/          # Imagenes PCX
├───── MAPS/            # Mapas de juego
├───── PALETTES/        # Paletas
├───── SETTINGS/        # Archivo de configuración
├───── SOUNDS/          # Sonidos
├───── SPRITES/         # Sprites PCX
├───── TEXT/            # Diálogos y textos
├───── TILESETS/        # Tilesets utilizados en los mapas
├───── UTILS/           # Utilidades de empaquetado y composición de música
├── SRC/                # Código fuente
├───── ENGINE/          # Código referente al motor de juego
├────────── ACTOR/         # Funciones del personaje principal
├────────── AUDIO/         # Funciones de sonido
├────────── BOSS/          # Funciones de gestión de jefes finales
├────────── BULLET/        # Funciones de gestión de balas
├────────── CAMERA/        # Funciones de manejo de cámara
├────────── EFFECT/        # Funciones de gestión de efectos gráficos
├────────── ENEMY/         # Funciones de gestión de enemigos
├────────── FILE/          # Funciones de manejo de archivos
├────────── GFX/           # Funciones de manejo de gráficos
├────────── GRENADE/       # Funciones de gestión de granadas
├────────── ITEM/          # Funciones de gestión de items
├────────── KEYB/          # Funciones de gestión de teclado
├────────── MAP/           # Funciones de gestión de mapas
├────────── MEM/           # Funciones de gestión de memoria
├────────── MOUSE/         # Funciones de gestión del ratón
├────────── NPC/           # Funciones de gestión de personajes no jugables
├────────── OBJECT/        # Funciones de gestión de objetos destruibles
├────────── PARTICLE/      # Funciones de gestión de partículas
├────────── TIMER/         # Funciones de gestión de temporizaciones
├────────── TYPES/         # Definiciones de tipos de datos
├────────── UI/            # Funciones de gestión de interfaces de usuario
├────────── UTILS/         # Funciones generales de cálculos comunies
├────────── VIDEO/         # Funciones de gestión de video
└── README.md
```

---

## 📸 Capturas

![Captura del juego](docs/screenshot1.png)

---

## 📜 Historia

Año 2045.

La ciudad está dominada por una secta que controla todos los aspectos de la sociedad.

Paconnor, un inspector de policía veterano y poco adaptado a los nuevos tiempos, recibe una misión que cambiará su vida.

Una máquina del tiempo. Una ciudad en decadencia. Y una última oportunidad para cambiar el futuro.

---

## 📋 Estado del desarrollo

| Sistema            | Estado |
| ------------------ | ------ |
| Motor gráfico      | ✅      |
| Scroll del mapa    | ✅      |
| Gestión de memoria | ✅      |
| Teclado            | ✅      |
| Ratón              | ✅      |
| Colisiones         | ✅      |
| IA enemiga         | ✅      |
| Sonidos Speaker    | ✅      |
| Música AdLib       | 🚧     |
| Sound blaster      | 🚧     |
| Historia           | 🚧     |

---

## 👨‍💻 Autor

**Manolete Games Company**

Desarrollado con pasión por los videojuegos retro y la programación de bajo nivel.
