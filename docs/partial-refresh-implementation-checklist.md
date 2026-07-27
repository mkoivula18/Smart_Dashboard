# Partial Refresh Implementation Checklist

Tämä checklist on tehty niin, että voit toteuttaa osapäivitykset vaiheittain itse ja samalla ymmärtää, mitä jokaisessa kohdassa oikeasti tapahtuu.

## 1. Ymmärrä nykyinen päivityspolku
- [ ] Varmista, että LVGL-display on nyt `LV_DISPLAY_RENDER_MODE_FULL`-tilassa.
- [ ] Varmista, että `Display::flush_callback(...)` ignoroi `area`-parametrin.
- [ ] Varmista, että ajuri lähettää koko framebufferin paneelille jokaisessa päivityksessä.
- [ ] Kirjoita itsellesi lyhyt muistiinpano: mitä dataa LVGL antaa `flush_callback`iin ja mitä ajuri odottaa.

## 2. Vaihda ajattelumalli full refreshistä area refreshiin
- [ ] Päätä yksi selkeä tavoite ensiversiolle: esimerkiksi "kellon tekstin päivitys päivittää vain kellon alueen".
- [ ] Pidä full refresh edelleen toimivana rinnakkaisena polkuna.
- [ ] Mieti valmiiksi, missä tilanteissa haluat aina full refreshin.
  - Esim. käynnistys
  - Esim. koko layoutin muuttuminen
  - Esim. ghostingin nollaus

## 3. Muuta LVGL partial-renderöintiin
- [ ] Vaihda `lv_display_set_buffers(...)` käyttämään `LV_DISPLAY_RENDER_MODE_PARTIAL`.
- [ ] Valitse draw-bufferin koko ensiversiolle.
  - Suositus: aloita yksinkertaisella suorakulmiopuskurilla, ei koko näytön puskurilla.
- [ ] Tarkista, miten `px_map` ja `area` liittyvät toisiinsa partial-tilassa.
- [ ] Lisää debug-printit ensivaiheessa, jotta näet mitkä alueet LVGL oikeasti flushaa.

## 4. Toteuta ajuriin aluekohtainen päivitys
- [ ] Lisää ajuriin uusi metodi aluepäivitykselle.
  - Esim. `DisplayRegion(...)`
- [ ] Toteuta alueen klippaus paneelin rajoihin.
- [ ] Toteuta x-koordinaattien kohdistus byte-rajoihin.
- [ ] Käsittele 1-bpp datan bittijärjestys ja nykyinen `~`-invertointi oikein vain lähetettävälle alueelle.
- [ ] Varmista, että lähetettävän datan rivipituus lasketaan alueen perusteella, ei koko ruudun perusteella.
- [ ] Pidä nykyinen `DisplayFrame(...)` tai vastaava full refresh -polku ennallaan.

## 5. Kytke `flush_callback` uuteen aluepäivitykseen
- [ ] Muuta `flush_callback(...)` käyttämään `area`-parametria.
- [ ] Kutsu ajurin uutta aluepäivitysmetodia `px_map`-datan kanssa.
- [ ] Kutsu `lv_display_flush_ready(display)` vasta sen jälkeen, kun alueen päivitys on valmis.
- [ ] Testaa debugilla, että pieni tekstimuutos ei enää lähetä koko ruutua.

## 6. Tee Display-tasolle helppo API widgetin päivitykseen
- [ ] Lisää `Display`-luokkaan metodi tyyliin `refresh_object(lv_obj_t* obj)`.
- [ ] Tee tarvittaessa rinnalle `refresh_area(...)`-apu.
- [ ] Päivitä layout ennen koordinaattien lukemista `lv_obj_update_layout(...)`-kutsulla.
- [ ] Hae objektin rajat `lv_obj_get_coords(...)`-kutsulla.
- [ ] Lisää pieni marginaali alueeseen, jos fontit/borderit muuten leikkaantuvat.
- [ ] Invalidoi alue tai objekti hallitusti ennen pakotettua refreshia.
- [ ] Pakota päivitys tarvittaessa `lv_refr_now(...)`-kutsulla.

## 7. Tee widgeteistä helpommin päivitettäviä
- [ ] Lisää `Widget`-baseen tapa palauttaa juurikontti.
  - Esim. `get_container()`
- [ ] Varmista, että jokaisella widgetillä on selkeä root-objekti, jonka alue voidaan päivittää.
- [ ] Pidä widgetien setterit yksinkertaisina: ne muuttavat sisällön, mutta eivät piilota päivityslogiikkaa liian syvälle.
- [ ] Päivitä yksi widget kerrallaan, aloita kellosta.

## 8. Tee ensimmäinen pystysuora slice valmiiksi
- [ ] Ota kellon minuuttipäivitys pilottitapaukseksi.
- [ ] Muuta kellon teksti.
- [ ] Päivitä vain kellon containerin alue.
- [ ] Tarkkaile, vilkkuuko koko ruutu vai vain kellon osa.
- [ ] Dokumentoi itsellesi mitä piti muuttaa, jotta juuri tämä toimi.

## 9. Lisää ghostingin hallinta
- [ ] Lisää osapäivityslaskuri `Display`-tasolle.
- [ ] Päätä ensiarvo testiä varten.
  - Suositus: tee full refresh esimerkiksi joka 10-30 osapäivityksen jälkeen.
- [ ] Lisää eksplisiittinen `refresh_full()`-polku.
- [ ] Varmista, että isot muutokset voivat edelleen käyttää full refreshia suoraan.
- [ ] Testaa, alkaako osapäivityksissä kertyä haamukuvaa.

## 10. Laajenna muille widgeteille
- [ ] Ota WiFi-widget seuraavaksi.
- [ ] Ota Weather-widget sen jälkeen.
- [ ] Testaa erityisesti tilanteet, joissa tekstin pituus muuttuu.
- [ ] Testaa tilanteet, joissa päivitettävä alue on lähellä ruudun reunoja.

## 11. Siivoa ja viimeistele
- [ ] Poista väliaikaiset debug-printit tai pienennä niiden määrää.
- [ ] Nimeä metodit niin, että niiden tarkoitus on heti selvä.
- [ ] Jätä full refresh ja partial refresh selkeästi eri poluiksi.
- [ ] Kirjoita lyhyt kommentti niihin kohtiin, joissa byte-alignment tai e-paper-spesifi logiikka ei ole ilmeinen.
- [ ] Päivitä tarvittaessa suunnitelmadokkarin viereen lyhyt "lessons learned" -muistiinpano.

## 12. Hyväksytty lopputulos
- [ ] Kellon päivitys ei vilkuta koko näyttöä.
- [ ] Pieni widgetin sisältömuutos päivittää vain paikallisen alueen.
- [ ] Full refresh on edelleen käytettävissä ja toimii varmasti.
- [ ] Ghosting pysyy hallinnassa ainakin ensiversion käytössä.
- [ ] Ymmärrät itse jokaisen kerroksen roolin: LVGL area, `flush_callback`, `Display`, ja EPD-ajuri.

## Hyvä työtapa tätä tehdessä
- [ ] Tee vain yksi muutoskerros kerrallaan.
- [ ] Testaa ensin debug-näkymän avulla mitä alueita oikeasti päivitetään.
- [ ] Aja ensin yksi onnistunut pilottitapaus loppuun ennen kuin yleistät kaikkea.
- [ ] Pidä toimiva full refresh aina varalla, jotta voit verrata käytöstä helposti.
