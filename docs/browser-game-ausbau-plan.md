# Ausbauplan für dein Browser-Spiel (Lucifer's Dominion)

## 1) Stabiler Kern (als erstes)

### Save/Load + Autosave
- Speichere `GameState.resources`, `buildings`, `population` (nur Daten, keine Meshes) in `localStorage`.
- Lade beim Start automatisch den letzten Spielstand.
- Autosave alle 30–60 Sekunden.

**Warum zuerst?**
Damit du neue Features bauen kannst, ohne dass Testläufe durch Neustarts verloren gehen.

### Datengetriebenes Balancing
- Werte aus `BUILDINGS` in eine JSON-Datei auslagern.
- Produktionsraten und Kosten dort definieren.
- Im Code nur noch lesen, nicht hardcoden.

**Warum?**
Schnelles Tuning ohne JS-Refactor.

---

## 2) Nächste große Features (hoher Impact)

### A) Straßen-/Brücken-Netz als echter Bonus
Aktuell gibts den Geschwindigkeitsbonus nur auf der aktuellen Tile-Prüfung. Nächster Schritt:
- Füge ein leichtes Pathfinding ein (A* auf Grid).
- Kantenkosten: Straße günstiger, Wasser blockiert (außer Brücke).
- Arbeiter suchen automatisch die schnellste Route.

### B) Produktionsketten
Erweitere von "Rohstoff direkt" auf Ketten:
- Mühle: Korn -> Mehl
- Bäckerei: Mehl + Holz -> Brot
- Markt verkauft Brot effizienter -> mehr Gold

Das schafft sinnvolle Midgame-Entscheidungen.

### C) Risiken/Events
- Feuer (Gebäude temporär offline)
- Dürre (Farm output -40%)
- Händlerfestival (Gold +50% für 2 Minuten)

Events halten das Spiel dynamisch.

---

## 3) Technische To-dos in deinem aktuellen Code

### A) Minimapslot im UI ergänzen
`drawMinimap()` sucht ein `#minimapCanvas`, im HTML ist aber keines vorhanden.

Einfügen in `#game-ui`:

```html
<div class="absolute bottom-6 right-6 glass-panel p-2 rounded-xl pointer-events-none">
  <canvas id="minimapCanvas" width="150" height="150" class="rounded"></canvas>
</div>
```

### B) Building-Placement Kollisionen verbessern
Beim Setzen von Gebäuden fehlt eine robuste Kollision gegen bestehende Gebäude (außer indirekt Geländechecks).
- Vor `placeBuilding()` eine Bounding-Check-Funktion ergänzen.
- Straßen ausnehmen oder überlappbar machen (je nach Design).

### C) Observer-Leaks vermeiden
Du registrierst pro Gebäude teilweise `onBeforeRenderObservable`-Callbacks.
- Bei `demolishSelected()` sicherstellen, dass gebäudespezifische Observer entfernt werden.

---

## 4) Priorisierte Roadmap (2 Wochen)

### Woche 1
1. Save/Load + Autosave
2. Minimap-Canvas fix
3. Building-Kollisionen
4. Balancing in externe JSON

### Woche 2
1. A* Pathfinding (nur Workers)
2. Produktionskette 1 (Farm -> Mühle -> Markt)
3. 2–3 Zufallsevents
4. Feinschliff UI-Feedback (Tooltips, rote Kosten wenn zu teuer)

---

## 5) Konkrete nächste Umsetzung (empfohlen)
Wenn du willst, bauen wir als **nächsten Schritt direkt Save/Load + Autosave** ein.

Das ist der größte Produktivitätsgewinn für die weitere Entwicklung.
