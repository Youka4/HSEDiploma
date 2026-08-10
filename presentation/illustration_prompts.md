# Промты для генерации иллюстраций в Claude Design

В презентации 4 рукописных иллюстрации на слайдах 5 и 6.
Ниже — промты для их перерисовки.

Общий стиль: чистая схематичная диаграмма, минималистичный стиль,
тёмно-синий (#1B2A5C) основной цвет, белый фон, без градиентов,
подходит для академической презентации. Линии чёткие, шрифт sans-serif.

---

## Иллюстрация 1 — Субаддитивность LCS (слайд 5, верхняя правая)

**Смысл:** Показать субаддитивность LCS для пары строк. Если разбить обе строки
на одинаковые куски и посчитать LCS попарно между соответствующими кусками,
сумма этих LCS будет ≤ LCS целых строк. Это даёт нижнюю оценку.

**Промт (НОВЫЙ, заменяет старый):**

```
A clean schematic diagram for an academic presentation on a white background.
The diagram illustrates subadditivity of LCS for a pair of strings.

TOP HALF — "by chunks":
Two horizontal parallel lines labeled "a" and "b". Both are divided into 4 equal
segments by vertical tick marks at the same positions. Between each corresponding
pair of segments (a_i on top, b_i on bottom), a vertical curly brace or double
arrow labeled "LCS_i". This shows LCS computed independently for each chunk pair.

BOTTOM HALF — "whole strings":
The same two lines "a" and "b" shown without divisions (full length). Between
them, a single large curly brace or double arrow labeled "LCS" spanning the
entire length.

TO THE RIGHT — the inequality:
"LCS_1 + LCS_2 + ... + LCS_k  ≤  LCS"
or equivalently: "Σ LCS_i  ≤  LCS(a, b)"

Colors: dark navy blue (#1B2A5C) for lines, labels, and text. Golden yellow
(#D4A843) for the chunk braces/arrows. Red (#CC3333) for the inequality symbol.
White background, no decorations, clean sans-serif font. No shadows or 3D.
```

---

## Иллюстрация 2 — Окна обзора на строках (слайд 5, нижняя правая)

**Что изображено сейчас:** две горизонтальные линии (строки a и b), на каждой
стрелка-указатель текущей позиции, от указателя вправо выделено окно из N символов
(красные скобки с буквой N), между строками показан сдвиг, пройденная часть строки
закрашена фиолетовым.

**Промт:**

```
A clean schematic diagram for an academic presentation on a white background.
Two horizontal parallel lines labeled "a" and "b" on the left, representing two
strings. On each line, a downward arrow marks the current reading position.
From each arrow to the right, a bracket spans N characters labeled "N" in red
(#CC3333). The portion of each string to the left of the arrow is highlighted
with a semi-transparent purple (#6B4C9A) overlay, representing the already-read
part. The arrows on lines a and b are at different horizontal positions, showing
a "shift" between them. A small green label "E" sits between the two purple
regions. Colors: dark navy (#1B2A5C) for lines and labels, red for the N brackets,
purple for the read portions. Minimal academic style, sans-serif font.
```

---

## Иллюстрация 3 — Состояние DP (слайд 6, нижняя левая)

**Что изображено сейчас:** две горизонтальные строки a и b. На строке a:
указатель позиции, от него вправо жёлтая скобка с надписью "maskA" над окном
из N символов (красные скобки с "N"). На строке b: указатель смещён вправо,
аналогичная синяя скобка "maskB". Между указателями подписано "shift" зелёным.
Над всей конструкцией жёлтая скобка "L" показывает оставшуюся длину.

**Промт:**

```
A clean schematic diagram for an academic presentation on a white background.
Two horizontal parallel lines labeled "a" (top) and "b" (bottom). Each line has
a downward black arrow marking the current position. On line "a", a bracket of
N characters labeled "maskA" in orange/yellow (#D4A843) sits above the string
starting from the arrow. A red bracket below labeled "N" marks the window size.
On line "b", the arrow is shifted to the right (offset from a's arrow). A blue
bracket labeled "maskB" (#3366CC) sits below line b. Between the two arrows,
a green label "shift" (#2D8C3C) with a double-headed horizontal arrow shows the
offset. Above everything, a large yellow curly brace spans from the arrows to
the end of the lines, labeled "L" for remaining length. Dark navy (#1B2A5C) for
main lines, clean sans-serif font. Minimal academic style.
```

---

## Иллюстрация 4 — Переход DP: продвижение строки a (слайд 6, нижняя правая)

**Что изображено сейчас:** аналогичная схема, но указатель на строке a сдвинут
на 1 позицию вправо (пунктирная стрелка показывает перемещение). maskA обновлена,
shift уменьшился на 1. Подписи: "maskA" (новая), "shift-1", "maskB" без изменений.
Жёлтая скобка "L-1" сверху.

**Промт:**

```
A clean schematic diagram for an academic presentation on a white background.
Same layout as the DP state diagram (two horizontal lines a and b), but showing
a TRANSITION: the arrow on line "a" has moved one position to the right. A small
dashed curved arrow above line a shows the movement from old position to new.
The orange bracket "maskA" is now shifted right and labeled "new maskA". The green
label between arrows now reads "shift - 1" showing the decreased offset. Line b's
arrow and blue bracket "maskB" remain unchanged. Above everything, a yellow curly
brace labeled "L - 1" (one less than before). Dark navy (#1B2A5C) for lines,
orange for maskA, blue for maskB, green for shift label. Minimal academic style,
sans-serif font, white background.
```

---

## Промты для доработки уже сгенерированных картинок 2-4

Картинки 2-4 получились неплохо, ниже — промты для точечных улучшений.

### Доработка иллюстрации 2 (windows):

```
Please fix this diagram. The green label "E" should not be on the left — remove
it entirely. The "shift" label with the double-headed arrow should be placed
BETWEEN the two downward arrows on lines a and b, showing the horizontal offset
between the current reading positions. Keep everything else the same.
```

### Доработка иллюстрации 3 (DP state):

```
Please fix this diagram. The green "shift" label with the double-headed arrow
should be placed BETWEEN the downward arrow on line "a" and the downward arrow
on line "b", showing the horizontal distance between the two reading positions.
Currently it appears between a's arrow and the red N bracket — move it so it
clearly shows the offset between a's position and b's position. Keep everything
else (L brace on top, maskA, maskB, N bracket) the same.
```

### Доработка иллюстрации 4 (DP transition):

```
Same fix as illustration 3: the green "shift - 1" label should clearly show
the horizontal distance between a's NEW arrow position and b's arrow position.
Keep the dashed curved arrow showing movement, "new maskA", "L - 1" brace,
and unchanged "maskB" as they are.
```

---

## Примечания

- Все иллюстрации должны быть в одном стиле
- Размер примерно 500x300 пикселей (формат слайда 16:9, иллюстрация занимает ~1/3)
- Если Claude Design не может выдержать точные цвета, основное — контраст
  между элементами (maskA != maskB != shift != основной текст)
- Буквы и надписи должны быть читаемы на слайде (не слишком мелко)
