2#!/usr/bin/env bash
# Сборка thesis: pdflatex -> biber -> pdflatex -> pdflatex
set -e

# Добавляем TeX в PATH (на случай, если ещё не подхватился)
if [ -d /Library/TeX/texbin ]; then
    export PATH="/Library/TeX/texbin:$PATH"
fi

cd "$(dirname "$0")"

echo "==> pdflatex (pass 1)"
pdflatex --shell-escape --interaction=nonstopmode main.tex

echo "==> biber"
biber main

echo "==> pdflatex (pass 2)"
pdflatex --shell-escape --interaction=nonstopmode main.tex

echo "==> pdflatex (pass 3)"
pdflatex --shell-escape --interaction=nonstopmode main.tex

echo ""
echo "Готово: $(pwd)/main.pdf"
