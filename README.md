![GTK+ 3 program in C screenshot](screenshot.png)

# Эксперименты с GTK+ 3 на Си и Обероне

*See English below*

## Установка (Дебиан)
```
sudo apt-get install libgtk-3-dev gcc make
```

## Компиляция и запуск
```
cd g001
make run
```

*Список примеров см. внизу*

-----------

# GTK+ 3 experiments in C and Oberon

## Setup (Debian)
```
sudo apt-get install libgtk-3-dev gcc make
```

## Compile & Run
```
cd g001
make run
```

*See the list of examples below*

-----------

## Примеры / Examples

| Имя / Name | Описание / Description |
| --------- | ----------- |
| [g001\_simplest](g001_simplest/gtk1.c) | Выводит маленькое окно с кнопкой. A small window with a button. |
| [g002\_cairo](g002_cairo/gtk2.c) | Соединяет указываемые мышью точки отрезками. Connects points (set with a mouse) with lines. |
| [g003\_tiles](g003_tiles/gtk3.c) | Рисует части картинки. Draws parts of an image. |
| [g004\_paint](g004_paint/gtk4.c) | Мышью быстро рисуются отрезки. Lines are quickly painted with a mouse. |
| [g005\_keyboard](g005_keyboard/gtk4.c) | В консоль выводятся нажимаемые клавиши. The pressed keys are printed to console. |
