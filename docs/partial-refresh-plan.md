# Osapäivitykset widget-/container-tasolla e-paperinäytölle

## Yhteenveto
Nykyinen vilkkuminen johtuu siitä, että LVGL renderöi `FULL`-tilassa ja `flush_callback` lähettää koko framebufferin paneelille jokaisessa päivityksessä. Toteutus muutetaan niin, että näyttö tukee osapäivitystä suorakulmioalueelle, mutta sovelluksen julkinen käyttöliittymä on objektipohjainen: voit pyytää päivityksen suoraan widgetille/containerille, ja taustalla järjestelmä laskee siitä pikselialueen ja lähettää vain sen alueen paneelille.

Hybridimalli otetaan käyttöön haamukuvien hallintaan: nopeat pienet muutokset, kuten kello, tehdään osapäivityksenä, mutta järjestelmä tekee ajoittain koko ruudun refreshin esimerkiksi tietyn osapäivitysmäärän jälkeen tai eksplisiittisesti, kun koko layout muuttuu.

## Keskeiset muutokset
- Muuta LVGL-display `FULL`-renderöinnistä `PARTIAL`-renderöintiin.
  - `lv_display_set_buffers(..., LV_DISPLAY_RENDER_MODE_PARTIAL)` käyttöön.
  - Käytä draw-bufferia, joka on pienempi kuin koko näyttö ja riittää aluekohtaiseen flushiin.
  - `flush_callback` alkaa käyttää `area`-parametria oikeasti eikä lähetä koko framea.

- Lisää EPD-ajuriin oikea aluepäivitys-API.
  - Uusi rajapinta tyyliin `DisplayRegion(const uint8_t* px_map, const lv_area_t& area)` tai vastaava rect-pohjainen metodi.
  - Ajuri vastaa alueen klippauksesta paneelin rajoihin.
  - X-koordinaatit kohdistetaan byte-rajoihin, koska 1-bpp data siirtyy 8 pikselin paloissa.
  - Lähetyslogiikka rajataan vain alueen riveihin ja byteihin; `~bit`-invertointi tehdään vain lähetettävälle osalle.
  - Vanha full refresh säilytetään erillisenä polkuna koko näytön päivityksiin.

- Lisää `Display`-luokkaan objektipohjainen päivitysrajapinta.
  - Uusi julkinen metodi tyyliin `refresh_object(lv_obj_t* obj)`.
  - Se tekee `lv_obj_update_layout(obj)` ennen koordinaattien lukemista.
  - Se hakee objektin koordinaatit `lv_obj_get_coords`-kutsulla, kasvattaa aluetta tarvittaessa pienellä marginaalilla ja invalidioi sen.
  - Päivitys pakotetaan heti `lv_refr_now(display)`-kutsulla, jotta juuri haluttu alue flushataan saman tien.
  - Haluttaessa rinnalle sisäinen `refresh_area(const lv_area_t&)`-apu, jota objektimetodi käyttää.

- Lisää widgeteille siisti tapa pyytää oma päivitys.
  - Helpoin malli: widgetin setterit vain muuttavat LVGL-objektin tekstiä, ja kutsuva koodi pyytää `Display`-tasolta kyseisen widgetin refreshausta.
  - Tätä varten `Display` tarvitsee pääsyn widgetin juurikonttiin; `Widget`-baseen kannattaa tarjota `get_container()` tai vastaava.
  - Kello käyttää tätä polkua minuuttipäivityksissä; suuremmat näkymämuutokset voivat edelleen käyttää full refreshia.

- Lisää hybridirefresh-policy ghostingin hallintaan.
  - `Display` pitää laskuria osapäivityksille.
  - Kun osapäivityksiä on tehty yli sovitun rajan, seuraava päivitys tehdään full refreshina ja laskuri nollataan.
  - Lisäksi tarjotaan eksplisiittinen `refresh_full()` esimerkiksi käynnistykseen, screen-vaihtoihin ja suuriin layoutmuutoksiin.

## Julkiset rajapinnat
- `Display`
  - Lisää objektipohjainen päivitysmetodi, esim. `refresh_object(lv_obj_t* obj)`.
  - Lisää koko ruudun päivitysmetodi, esim. `refresh_full()`.
  - Lisää mahdollinen sisäinen tai julkinen rect-pohjainen fallback, esim. `refresh_area(const lv_area_t&)`.

- `Widget`
  - Lisää tapa palauttaa widgetin juurikontti, esim. `lv_obj_t* get_container() const`.
  - Tätä käytetään, jotta `Display` voi päivittää juuri kyseisen widgetin alueen.

- EPD-ajuri
  - Lisää aluekohtainen renderöintimetodi, joka ottaa alueen koordinaatit ja LVGL:n toimittaman pikselidatan.
  - Säilytä nykyinen full-frame-metodi erillisenä.

## Testit ja hyväksymiskriteerit
- Kellon tekstin vaihto päivittää vain kellowidgetin alueen eikä aiheuta koko näytön vilkkumista.
- WiFi-statuksen tekstin vaihto päivittää vain WiFi-widgetin alueen.
- Sää-widgetin numeron vaihto päivittää vain sääalueen, myös silloin kun tekstin leveys muuttuu.
- Osapäivitys toimii oikein myös silloin, kun objektin leveys ei osu 8 pikselin byte-rajaan.
- Alue näytetään oikein ruudun reunoilla ilman ylivuotoa tai väärää klippausta.
- Hybridimallissa full refresh laukeaa odotetusti määritellyn osapäivitysmäärän jälkeen.
- Ensikäynnistys ja isot layoutmuutokset käyttävät edelleen full refreshia, joten alkuperäinen toimivuus ei regressioidu.

## Oletukset ja valitut oletusarvot
- Ensisijainen käyttö-API on container-/objektipohjainen; rect-pohjainen API on tekninen apurajapinta sen alla.
- Ghostingin hallinta tehdään hybridimallilla, ei pelkillä osapäivityksillä.
- Full refresh pidetään käytössä käynnistyksessä ja suurissa näkymämuutoksissa.
- Toteutuksessa ei tehdä yleistä "päivitä mikä tahansa automaattisesti ilman kutsua" -arkkitehtuuria, vaan sovelluskoodi pyytää kohdepäivityksen selvästi niissä kohdissa, joissa widgetin sisältö muuttuu.
- Jos 7.5" Waveshare V2 -paneelin nykyinen init-sekvenssi ei yksin riitä vakaaseen true partial refreshiin, suunnitelmaan kuuluu ajurin komentosekvenssin tarkennus kyseisen paneelin partial update -moodiin ennen käyttöönottoa; tätä ei jätetä toteuttajan päätettäväksi vaan se tutkitaan ajurimuutoksen yhteydessä.
