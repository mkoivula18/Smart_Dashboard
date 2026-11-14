#include "WeatherStation.hpp"
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <cmath>

static void start_element(void *user_data, const XML_Char *name, const XML_Char **attrs);
static void character_data(void *user_data, const XML_Char *text, int len);
static void end_element(void *user_data, const XML_Char *name);


struct ParserState
{
    std::string current_tag;
    std::string current_text;

    std::string latest_time;
    std::string latest_temperature;
};

std::string WeatherStation::get_temperature_xml()
{
    std::string xml = fetch_xml_from_url(FMI_URL.c_str());
    ESP_LOGD(TAG, "XML length: %d", xml.size());
    // ESP_LOGI(TAG, "%s", xml.c_str());

    if (xml.empty())
    {
        ESP_LOGW(TAG, "No XML received, skipping parse");
        return "";
    }

    ParserState parser_state{}; 

    XML_Parser parser = XML_ParserCreate(nullptr);

    XML_SetUserData(parser, &parser_state);
    XML_SetElementHandler(parser, start_element, end_element);
    XML_SetCharacterDataHandler(parser, character_data);

    XML_Status result = XML_Parse(parser, xml.c_str(), xml.size(), XML_TRUE);

    if (result == XML_STATUS_ERROR) {
        ESP_LOGE(TAG, "XML parse error: %s",
                 XML_ErrorString(XML_GetErrorCode(parser)));

        XML_ParserFree(parser);
        return "";
    }

    if (parser_state.latest_temperature.empty())
    {
        ESP_LOGW(TAG, "XML parsed but no temperature value was found");
        XML_ParserFree(parser);
        return "";
    }

    // Turn result from float to integer
    int temp = (int)std::round(std::stof(parser_state.latest_temperature));

    parser_state.latest_temperature = std::to_string(temp);

    ESP_LOGD(TAG, "Latest time: %s, Latest temperature: %sC",
             parser_state.latest_time.c_str(),
             parser_state.latest_temperature.c_str());

    XML_ParserFree(parser);

    return parser_state.latest_temperature;
}


void start_element(void *user_data, const XML_Char *name, const XML_Char **attrs)
{
    ParserState *parser_state = (ParserState*)user_data;

    parser_state->current_tag = name;
    parser_state->current_text.clear();
}

void character_data(void *user_data, const XML_Char *text, int len)
{
    ParserState *parser_state = (ParserState*)user_data;

    if (parser_state->current_tag == "wml2:time" || parser_state->current_tag == "wml2:value")
    {
        parser_state->current_text.append(text, len);
    }
}

void end_element(void *user_data, const XML_Char *name)
{
    ParserState *parser_state = (ParserState*)user_data;

    if (strcmp(name, "wml2:time") == 0)
    {
        parser_state->latest_time = parser_state->current_text;
    }
    
    if (strcmp(name, "wml2:value") == 0)
    {
        parser_state->latest_temperature = parser_state->current_text;
    }

    parser_state->current_tag.clear();
    parser_state->current_text.clear();
}
