# 🕵️ PACONNOR, El tiempolicia

> **Un shooter de acción y aventura para MS-DOS, ambientado en un futuro distópico.**

[![Platform](https://img.shields.io/badge/Platform-MS--DOS-555555)](https://github.com/ManoleteGames/paconnor-el-tiempolicia)
[![Language](https://img.shields.io/badge/Language-C-blue)](https://github.com/ManoleteGames/paconnor-el-tiempolicia)
[![Status](https://img.shields.io/badge/Status-In%20Development-orange)](https://github.com/ManoleteGames/paconnor-el-tiempolicia)

Puedes descargar el último *release* y utilizarlo en emuladores como **DOSBox, PCem o 86Box**.

También puedes jugar directamente en **[Itch.io](https://manoletegames.itch.io/paconnor-el-tiempolicia)**.

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

**PACONNOR, El tiempolicia** es un videojuego de acción para **MS-DOS**, inspirado en clásicos como *The Chaos Engine*, *True Lies* y *Demolition Man*.

El jugador controla a un hastiado inspector de policía que debe salvar el mundo y liberarlo de una secta que lo controla todo.

El proyecto está desarrollado en **C con DJGPP**, utilizando gráficos **VGA de 320×200 píxeles**.

---

## ✨ Características

* 🎯 Acción con teclado y ratón
* 🕹️ Perspectiva cenital (*top-down*)
* 🖥️ Gráficos VGA de 320×200
* 💾 Diseñado para MS-DOS
* 🔫 Armas, enemigos y proyectiles
* 🤖 Ciudad futurista y decadente
* ⏳ Viajes en el tiempo
* 🎵 Música y efectos de sonido retro
* 🔊 Soporte para PC Speaker

---

## 🕹️ Controles

| Acción         | Tecla               |
| -------------- | ------------------- |
| Moverse        | **W A S D**         |
| Esquivar       | **SPACE**           |
| Apuntar        | 🖱️ Ratón           |
| Disparar       | 🖱️ Botón izquierdo |
| Lanzar granada | 🖱️ Botón derecho   |
| Recargar arma  | **R**               |
| Puños          | **1**               |
| Pistola        | **2**               |
| Escopeta       | **3**               |
| UZI            | **4**               |
| Rifle          | **5**               |
| Pausa          | **P**               |
| Salir          | **ESC**             |

---

## 🖥️ Requisitos

* 💻 PC compatible con MS-DOS
* ⚙️ Procesador **486 DX2 66 MHz** o superior
* 🖥️ Tarjeta gráfica **VGA**
* 🖱️ Ratón compatible con MS-DOS
* 🔊 **PC Speaker**
* 💾 **16 MB de RAM**

---

## 🛠️ Compilación

El proyecto utiliza **DJGPP** para generar el ejecutable compatible con DOS.

El código fuente está escrito en **C**, utilizando técnicas y tecnologías orientadas al hardware de la época.

---

## 📁 Estructura del proyecto

```text
PACONNOR/
├── ASSETS/                 # Recursos del juego
│   ├── BINARIES/           # Pantallas en modo texto
│   ├── FONTS/              # Fuentes de texto
│   ├── IMAGES/             # Imágenes PCX
│   ├── MAPS/               # Mapas del juego
│   ├── PALETTES/           # Paletas de colores
│   ├── SETTINGS/           # Archivos de configuración
│   ├── SOUNDS/             # Sonidos
│   ├── SPRITES/            # Sprites PCX
│   ├── TEXT/               # Diálogos y textos
│   ├── TILESETS/           # Tilesets utilizados en los mapas
│   └── UTILS/              # Utilidades de empaquetado y composición musical
│
├── SRC/                    # Código fuente
│   └── ENGINE/             # Motor del juego
│       ├── ACTOR/          # Personaje principal
│       ├── AUDIO/          # Sistema de sonido
│       ├── BOSS/           # Gestión de jefes finales
│       ├── BULLET/         # Gestión de balas
│       ├── CAMERA/         # Manejo de cámara
│       ├── EFFECT/         # Efectos gráficos
│       ├── ENEMY/          # Gestión de enemigos
│       ├── FILE/           # Manejo de archivos
│       ├── GFX/            # Sistema gráfico
│       ├── GRENADE/        # Gestión de granadas
│       ├── ITEM/           # Gestión de objetos
│       ├── KEYB/           # Gestión del teclado
│       ├── MAP/            # Gestión de mapas
│       ├── MEM/            # Gestión de memoria
│       ├── MOUSE/          # Gestión del ratón
│       ├── NPC/            # Personajes no jugables
│       ├── OBJECT/         # Objetos destruibles
│       ├── PARTICLE/       # Sistema de partículas
│       ├── TIMER/          # Temporizadores
│       ├── TYPES/          # Definiciones de tipos de datos
│       ├── UI/             # Interfaz de usuario
│       ├── UTILS/          # Funciones y cálculos generales
│       └── VIDEO/          # Gestión de vídeo
│
└── README.md
```

---

## 📜 Historia

### Año 2045

El mundo ya no es el mismo.

La ciudad está dominada por una secta que controla todos los aspectos de la sociedad. El gobierno, las grandes corporaciones y buena parte de la población están bajo su influencia.

**Paconnor**, un veterano inspector de policía poco adaptado a los nuevos tiempos, recibe una misión que cambiará su vida.

Una máquina del tiempo.

Una ciudad en decadencia.

Y una última oportunidad para cambiar el futuro.

---

## 📋 Estado del desarrollo

| Sistema               | Estado |
| --------------------- | :----: |
| 🎨 Motor gráfico      |    ✅   |
| 🗺️ Scroll del mapa   |    ✅   |
| 💾 Gestión de memoria |    ✅   |
| ⌨️ Teclado            |    ✅   |
| 🖱️ Ratón             |    ✅   |
| 💥 Colisiones         |    ✅   |
| 🤖 IA enemiga         |    ✅   |
| 🔊 Sonidos PC Speaker |    ✅   |
| 🎵 Música AdLib       |   🚧   |
| 🔊 Sound Blaster      |   🚧   |
| 📜 Español            |   ✅   |
| 📜 Inglés             |   ✅   |
| 📜 Francés            |   ✅   |
| 📜 Alemán             |   🚧   |

### Leyenda

* ✅ **Completado**
* 🚧 **En desarrollo**
* ⬜ **Pendiente**

---

## 👨‍💻 Autor

### Manolete Games Company

Desarrollado con pasión por los **videojuegos retro**, la **programación de bajo nivel** y el hardware clásico.

---

<p align="center">
  <b>Made with ❤️ for MS-DOS</b>
</p>
