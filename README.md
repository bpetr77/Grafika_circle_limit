Feladat: Vegyünk fel egy „sakktáblát” a hiperbolikus síkon az alábbiak szerint. A Minkowski térbe ágyazott gömb
északi pólusától indulva (ami egy euklideszi turista számára a kétköpenyű hiperboloid felső köpenyének
az alja), a +x irányba lépjünk el először 𝑑𝐻/2 távolságot, majd pedig további ötször lépjünk 𝑑𝐻 távolságot,
ahol 𝑑𝐻 = 1. Az egyes lépéseknél az adott haladási irányra merőlegesen vegyünk fel egy hiperbolikus
egyenest. Az eljárást ismételjük meg még nyolcszor, az x tengellyel 40 fokonként növekvő irányokban
(azaz egy kilencszög oldalai felé terjeszkedünk). Az egyenesek a hiperbolikus síkon cellákat választanak
szét. A sakktáblához színezzük ki a cellák felét úgy, hogy két élszomszédos cella közül az egyik maximális
intenzitású kék, a másik maximális intenzitású sárga legyen. A hiperboloid alját tartalmazó cella sárga.
A hiperbolikus síkon keletkező ábrát a Poincaré diszkre vetítve jelenítjük meg. A Poincaré körlemez fekete
négyzetbe foglalt képe lesz a textúra. A textúra kezdeti felbontása 300x300-as és GL_LINEAR szűrési módot használunk. Az ‘r’ billentyű
lenyomásának hatására a lineáris felbontás 100-al csökken, az ’R’ hatására 100-al nő. A ‘t’-vel
GL_NEAREST módot állíthatunk, a ‘T’-vel pedig GL_LINEAR-t.

Az euklideszi geometriát követő virtuális világban a távolságot tengeri mérföldben, azaz [tm]-ben mérjük.
A virtuális világ egyetlen négycsúcsú „csillagból” áll, melynek középpontja a (50, 30) pontban van, a
hosszúsága és magassága 80 tm. Az élközép pontok a csillag középpontjától 𝑠 tm-re vannak, ahol az 𝑠
alapértelmezésben 40, és a ‘h’ billentyű lenyomásának hatására 10-zel csökken (azaz a négyzetből
indulunk és a ‘h’ nyomogatásával a csillag egyre hegyesebbé válik). Erre a csillagra textúrázzuk a Poincaré
kört befoglaló négyzetben definiált képet.
A csillag geometriája az alábbi ábrán látható, ahol az egyes pontokhoz zárójelben azt is feltüntettük, hogy
ezek a pontok hol vannak textúra koordinátarendszerben. Az ‘s’ értéke nem befolyásolja a textúrázás
koordinátáit, csak a csillag geometriáját.
A csillag középpontja egy körpályán mozog az (50, 30) pontból indulva, a teljes kört 10 sec alatt megtéve.
A körpálya középpontja a (20, 30)-as pont és sugara 30. Eközben a csillag a saját középpontja körül forog
0.2 sec-1
fordulatszámmal. A csillag animációja az ‘a’ lenyomásával indítható.
A kameraablak középpontja a világkoordinátarendszer (20, 30)-as pontja, a mérete 150 tm x 150 tm. A
fénykép (viewport) a teljes 600 x 600 pixel felbontású alkalmazói ablakot lefedi.
