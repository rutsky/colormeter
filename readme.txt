ColorMeter
----------

1. Сборка программы
-------------------

Для сборки программы необходима библиотека Qt4 и компилятор MinGW.
Установочный дистрибутив Qt можно найти на 
http://trolltech.com/developer/downloads/qt/windows,
он также предложит автоматически скачать нужную версию компилятора MinGW.

Для компиляции выполните в корневой директории проекта:
  qmake
  mingw32-make
  
В случае успешной компиляции будут создан выполняемый файл в диретории bin.

Примечание: в директории win32 находится заранее собранная версия программы
для Windows, с минимальным набором необходимых для работы библиотек. Она 
может работать без исходных текстов и прочих файлов.

2. Алгоритм работы
------------------

Программа считает абсолютно черный и абсолютно белый цвета прозрачными, они 
не учитываются при подчете статистики.

Результатом работы программы является таблица зависимости статистики цветов 
пикселей изображении, в зависимости от шага фильтрации, и график 
визуализирующий информацию из этой таблицы.

Работа программы заключается в последовательном сокращении количества цветов 
в палитре, согласно указанному ниже правилу.
При сборе статистики на каждом шаге используются только цвета пикселей из 
палитры данного шага.

Статистика цветов содержит в себе согласно таблице для каждого шага фильтрации:
    "Pixel %"           
       - процент количества пикселей для данной палитры по отношению к 
       оригинальному изображению.
    "Colors in pallete"
       - количество различных цветов в данной палитре.
    "Minimum", "Maximum", "Average"
       - минимальное, максимальное, и среднее значение цвета для данной 
       палитры.
       Данные значения считаются независимо для каждой компоненты (т.е. для 
       всех пикселей ищется минимальное значение, например, красной 
       компоненты, и она обзывается за "R" "Minimum"; "R" "Average" --- это 
       сумма значений красных компонент всех пикселей, поделённое на их 
       число), возможно, что пиксела с данным цветом из таблицы реально на 
       изображении нет.
       "R", "G", "B" --- это соответствующие значения, а "Value" --- значение 
       в 32х-битном представление цвета, оно равно (R * 65536 + G * 256 + B).
    "Standart deviation"
       - среднеквадратичное отклонение каждой компоненты для данной палитры. 
       Вычисляется отдельно для каждой компоненты цвета, как 
       sqrt(1/n * summ(x_i - x_avg)^2), где n --- это количество пикселей, 
       суммирование идет по всем пикселям данной палитры, x_i --- значение 
       цвета выбранной компоненты у i-того пикселя, x_avg --- среднее значение 
       цвета данной компоненты по всем пикселям данной палитры.
       Значение "Value" вычисляется также как и для "Minimum", "Maximum" и 
       "Average".
    "Average color"
       - показывает средний цвет ("Average").

Шаг фильтрации состоит в том, что из палитры (набора пикселей) удаляются все 
пиксели, хотя бы одна компонента которых оказывается вне отрезка 
["Average color" - "Standart deviation", "Average color" + "Standart deviation"]
(покомпонентно).
Фильтрация ведется пока в палитре не останется 1 пиксель. Если в результате 
фильтрации выпадают все пиксели, то в качестве финальной палитры берется 
палитра, состоящая из одного пикселя --- произвольного пикселя из последней 
палитры.

3. Комментарии
--------------

Проведение каких либо исследований на отдельных компонентах цвета в 
покомпонентном разложении в какой-либо цветовой схеме (RGB, CMYK) является 
некорректным, хотя бы потому, что распределение цвета по отдельным компонентам
не равноверятно.
