Pokud je požadováno přemístění nákladu z jednoho místa do druhého, vozík si materiál vyzvedne do 1 minuty.

- AMB_STATEMENT, je požadováno přemístění -> je zadán požadavek na přesun, "je požadováno přemístění" nespecifikuje přesně akci zadání požadavku.
- AMB_STATEMENT, náklad -> materiál, Sjednocení pojmů.
- AMB_STATEMENT, místo -> zastávka, Sjednocení pojmů.
- AMB_TEMPORAL, Není přesně určeno: Do minuty od čeho bude materiál vyzvednut?

*Pokud je zadán požadavek na přesun materiálu z jedné zastávky do druhé, vozík si materiál do 1 minuty od zadání požadavku na přesun vyzvedne.*

Pokud se to nestihne, materiálu se nastavuje prioritní vlastnost.

- AMB_REFERENCE, Není jednoznačné, co "to" označuje.
- AMB_STATEMENT, Není zde uvedeno, k čemu se "prioritní vlastnost" vztahuje a co znamená.

*Pokud vozík nestihne vyzvednou materiál do 1 minuty od zadání požadavku na přesun, je zadán prioritní požadavek na přesun materiálu.*

Každý prioritní materiál musí být vyzvednutý vozíkem do 1 minuty od nastavení prioritního požadavku.

- AMB_STATEMENT, Každý prioritní materiál -> materiál s prioritním požadavkem na přesun. Sjednocení pojmů.
- AMB_STATEMENT, nastavení -> zadání, Sjednocení pojmů.
- DANGLING_ELSE, Chybí Else. Co se stane, když nebude materiál s prioritním požadavkem na přesun vyzvednut vozíkem do 1 minuty od zadání prioritního požadavku. Nechť je vyvolána VÝJIMKA.
- OMISSION, Chybí důsledek.

*Pokud je materiál se zadaným prioritním požadavkem na přesun vyzvednut vozíkem do 1 minuty, pak bude materiál s prioritním požadavkem na přesun naložen na vozík. Pokud není materiál se zadaným prioritním požadavkem na přesun vyzvednut vozíkem do 1 minuty, bud vyvolána VÝJIMKA.*

Pokud vozík nakládá prioritní materiál, přepíná se do režimu pouze-vykládka.

- AMB_STATEMENT, nakládá -> vyzvedá, Sjednocení pojmů.
- AMB_STATEMENT, prioritní materiál -> materiál s prioritním požadavkem na přesun, Sjednocení pojmů.
- DANGLING_ELSE, Chybí Else. Co když žádný taková materiál nenakládá?

*Pokud vozík vyzvedává materiál s prioritním požadavkem na přesun, pak se vozík přepíná do režimu pouze-vykladka. Pokud vozík vyzvedává materiál bez prioritního požadavku na přesun, pak se vozík nepřepíná do režimu pouze-vykladka.*

V tomto režimu zůstává, dokud nevyloží všechen takový materiál.

- AMB_REFERENCE, "tomto", Vzniká pochyba o jaký režim se jedná.
- AMB_REFERENCE, "takový", Vzniká pochyba o jaký materiál se jedná.

*V režimu pouze-vykladka zůstává vozík, dokud nevyloží všechen materiál s prioritním požadavkem na přesun.*

Normálně vozík během své jízdy může nabírat a vykládat další materiály v jiných zastávkách.

- IMPLICIT, Není určeno, co "normálně" (nejspíš normální režim) znamená.
- AMB_STATEMENT, nabírat -> vyzvedávat, Sjednocení pojmů.
- IMPLICIT, Nemusí být jasné, které jsou "jiné zastávky".
- DANGLING_ELE, Chybí Else. Co když je vozík v režimu pouze-vykladka?

*Pokud není vozík v režimu pouze-vykladka, může vyzvedávat materiály na libovolných zastávkách a vykládat materiály na libovolných cílových stanicích vyzvednutých materiálů. Pokud je vozík v režimu pouze-vykladka, nemůže vyzvedávat materiál a může vykládat materiály na libovolných cílových stanicích vyzvednutých materiálů.*

Na jednom místě může vozík akceptovat nebo vyložit jeden i více materiálů.

- AMB_STATEMENT, místo -> zastávka, Sjednocení pojmů.
- AMB_STATEMENT, akceptovat -> vyzvednout, Sjednocení pojmů.
- AMB_LOGIC, Spojka "nebo" může zpusobit chybnou interpretaci. (A to i přesto, že je užita gramaticky správně.)

*Na jedné zastávce může vozík vyzvednou i vyložit jeden i více materiálů.*

Pořadí vyzvednutí materiálů nesouvisí s pořadím vytváření požadavků.

- AMB_STATEMENT, vytváření požadavku -> zadání požadavku na přesun, Sjednocení pojmů.
- IMPLICIT, Není přesně určeno, o jaký požadavek se jedná.

*Pořadí vyzvednutí materiálů nesouvisí s pořadím zadávání požadavků na přesun materiálů.*

Vozík neakceptuje materiál, pokud jsou všechny jeho sloty obsazené nebo by jeho převzetím byla překročena maximální nosnost.

- AMB_STATEMENT, neakceptuje -> nevyzvedne, Sjednocení pojmů.
- AMB_REFERENCE, "jeho" vs. "jeho", Nemusí být jasné, zda se jedná o vozík, nebo materiál.
- UNSPECIFIED_SUBJECT, maximální nosnost, Nemusí být jasné, že se jedná o maximální nosnost vozíku.
- OTHER, Zbytečná negace.

*Vozík vyzvedne materiál, pokud má pro materiál odpovídající množství volných slotů, a pokud jeho vyzvednutím nebude překročena maximální nosnost vozíku.*
