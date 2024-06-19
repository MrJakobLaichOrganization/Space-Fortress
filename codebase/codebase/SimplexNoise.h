/*
* the github is listed as MIT
* link to github: https://github.com/SRombauts/SimplexNoise
*
* The MIT License (MIT)

Copyright (c) 2012-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// copied straight from the github. (lmao) (copied, again, straight from my own codebase, which copies from the github)
// this is a modified version of the original codebase

/**
 * @file    SimplexNoise.h
 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).
 *
 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <array>
#include <cstddef>  // size_t
#include <map>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * @brief A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D, 4D).
 */
class SimplexNoise {
public:
    // 1D Perlin simplex noise
    float noise(float x);
    // 2D Perlin simplex noise
    float noise(float x, float y);
    // 3D Perlin simplex noise
    float noise(float x, float y, float z);

    // Fractal/Fractional Brownian Motion (fBm) noise summation
    float fractal(size_t octaves, float x);
    float fractal(size_t octaves, float x, float y);
    float fractal(size_t octaves, float x, float y, float z);

    std::array<uint8_t, 256> getSeed();
    void setSeed(std::array<uint8_t, 256>);
    void Randomize(int* S = 0);

    int32_t fastfloor(float fp);
    uint8_t hash(int32_t i);
    float grad(int32_t hash, float x);
    float grad(int32_t hash, float x, float y);
    float grad(int32_t hash, float x, float y, float z);

    std::vector<float> getOffsets();
    void setO(float iO, float aO, float X, float Y, float Z);

    /**
     * Constructor of to initialize a fractal noise summation
     *
     * @param[in] frequency    Frequency ("width") of the first octave of noise (default to 1.0)
     * @param[in] amplitude    Amplitude ("height") of the first octave of noise (default to 1.0)
     * @param[in] lacunarity   Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).
     * @param[in] persistence  Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)
     */
    SimplexNoise(float frequency = 1.0f,
        float amplitude = 1.0f,
        float lacunarity = 2.0f,
        float persistence = 0.5f) :
        mFrequency(frequency),
        mAmplitude(amplitude),
        mLacunarity(lacunarity),
        mPersistence(persistence) {
    }

    // Parameters of Fractional Brownian Motion (fBm) : sum of N "octaves" of noise
    float mFrequency = 1.0f;   ///< Frequency ("width") of the first octave of noise (default to 1.0)
    float mAmplitude = 1.0f;   ///< Amplitude ("height") of the first octave of noise (default to 1.0)
    float mLacunarity = 2.0f;  ///< Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).
    float mPersistence = 0.5f; ///< Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)
    //std::array<uint8_t, 256> seed = { 151, 160, 137, 91, 90, 15,
    //    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    //    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    //    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    //    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    //    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    //    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    //    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    //    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    //    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    //    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    //    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    //    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    //};
    float minO = -1000.0f;
    float maxO = 1000.0f;
    float oX = 0.0f;
    float oY = 0.0f;
    float oZ = 0.0f;
private:
    std::array<uint8_t, 256> seed = { 151, 160, 137, 91, 90, 15,
        131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
        190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
        88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
        77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
        102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
        135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
        5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
        223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
        129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };
};

struct Limit
{
    float MIN;
    float MAX;

    friend bool operator<(const Limit& L, const Limit& R)
    {
        return L.MIN < R.MIN;
    }
    friend bool operator==(const Limit& L, const Limit& R)
    {
        return L.MIN == R.MIN && L.MAX == R.MAX;
    }
    friend bool operator!=(const Limit& L, const Limit& R)
    {
        return !(L == R);
    }
};

// this needs to be cleaned up!

class Noise
{
public:
    std::string noiseName = "";
    SimplexNoise simplex;
    int octaves = 1;
    float minValue = -1.0f;
    float maxValue = 1.0f;
    json noiseData = {};
    std::map<Limit, std::string> limits;
    //overwrite data values
    bool fillNoiseData = false;
    std::string fillType = "Air";
    bool fillEmptySlots = false;
    std::vector<std::string> whitelist = {};
    std::vector<std::string> blacklist = {};
    float scaleParameter = 1.0f;
    bool replaceColliding = false;
    bool forceReplaceAll = false;
    bool ignoreColliding = false;
    int placeOnly = -1;
    float probability = 1.0f;

    void fromJSON(json& J)
    {
        noiseData = J["noiseData"];

        if (J.contains("noiseMeta"))
        {
            json& meta = J["noiseMeta"];
            if (meta.contains("noiseName"))
            {
                noiseName = meta["noiseName"];
            }
            if (meta.contains("probability"))
            {
                probability = meta["probability"];
            }
            if (meta.contains("scaleParameter"))
            {
                scaleParameter = meta["scaleParameter"];
            }
            if (meta.contains("limits"))
            {
                std::map<Limit, std::string> limits_ = {};
                for (auto& Leach : meta["limits"].items())
                {
                    std::string limitName = Leach.key();
                    std::string limitJSON = Leach.value();
                    int commaPos = limitName.find(",");
                    Limit limit = { std::stof(limitName.substr(0, commaPos)), std::stof(limitName.substr(commaPos + 1)) };
                    limits_[limit] = limitJSON;
                }
                limits = limits_;
            }
            if (meta.contains("seed"))
            {
                simplex.setSeed(meta["seed"]);
            }
            if (meta.contains("minValue"))
            {
                minValue = meta["minValue"];
            }
            if (meta.contains("maxValue"))
            {
                maxValue = meta["maxValue"];
            }
            if (meta.contains("scale"))
            {
                simplex.mFrequency = meta["scale"];
            }
            if (meta.contains("amplitude"))
            {
                simplex.mAmplitude = meta["amplitude"];
            }
            if (meta.contains("lacunarity"))
            {
                simplex.mLacunarity = meta["lacunarity"];
            }
            if (meta.contains("persistance"))
            {
                simplex.mPersistence = meta["persistance"];
            }
            if (meta.contains("octaves"))
            {
                octaves = meta["octaves"];
            }
            if (meta.contains("offsets") && meta["offsets"].size() > 0)
            {
                simplex.minO = meta["offsets"][0];
            }
            if (meta.contains("offsets") && meta["offsets"].size() > 1)
            {
                simplex.maxO = meta["offsets"][1];
            }
            if (meta.contains("offsets") && meta["offsets"].size() > 2)
            {
                simplex.oX = meta["offsets"][2];
            }
            if (meta.contains("offsets") && meta["offsets"].size() > 3)
            {
                simplex.oY = meta["offsets"][3];
            }
            if (meta.contains("offsets") && meta["offsets"].size() > 4)
            {
                simplex.oZ = meta["offsets"][4];
            }
        }

        if (J.contains("overwriteData"))
        {
            json& overwrite = J["overwriteData"];
            if (overwrite.contains("fillNoiseData"))
            {
                fillNoiseData = overwrite["fillNoiseData"];
            }
            if (overwrite.contains("fillType"))
            {
                fillType = overwrite["fillType"];
            }
            if (overwrite.contains("fillEmptySlots"))
            {
                fillEmptySlots = overwrite["fillEmptySlots"];
            }
            if (overwrite.contains("whitelist"))
            {
                whitelist = overwrite["whitelist"];
            }
            if (overwrite.contains("blacklist"))
            {
                blacklist = overwrite["blacklist"];
            }
            if (overwrite.contains("replaceColliding"))
            {
                replaceColliding = overwrite["replaceColliding"];
            }
            if (overwrite.contains("forceReplaceAll"))
            {
                forceReplaceAll = overwrite["forceReplaceAll"];
            }
            if (overwrite.contains("ignoreColliding"))
            {
                ignoreColliding = overwrite["ignoreColliding"];
            }
            if (overwrite.contains("placeOnly"))
            {
                placeOnly = overwrite["placeOnly"];
            }
        }
    }

    json getJSON()
    {
        json noiseJSON = {};
        noiseJSON["noiseData"] = noiseData;
        noiseJSON["noiseMeta"] = {};
        noiseJSON["noiseMeta"]["noiseName"] = noiseName;
        noiseJSON["noiseMeta"]["octaves"] = octaves;
        noiseJSON["noiseMeta"]["scale"] = simplex.mFrequency;
        noiseJSON["noiseMeta"]["amplitude"] = simplex.mAmplitude;
        noiseJSON["noiseMeta"]["lacunarity"] = simplex.mLacunarity;
        noiseJSON["noiseMeta"]["persistance"] = simplex.mPersistence;
        noiseJSON["noiseMeta"]["seed"] = simplex.getSeed();
        noiseJSON["noiseMeta"]["offsets"] = simplex.getOffsets();
        noiseJSON["noiseMeta"]["minValue"] = minValue;
        noiseJSON["noiseMeta"]["maxValue"] = maxValue;
        noiseJSON["noiseMeta"]["scaleParameter"] = scaleParameter;
        noiseJSON["noiseMeta"]["probability"] = probability;
        noiseJSON["noiseMeta"]["limits"] = {};
        for (auto Leach : limits)
        {
            Limit limit = Leach.first;
            std::string L = std::to_string(limit.MIN) + "," + std::to_string(limit.MAX);
            json limitJSON = Leach.second;
            noiseJSON["noiseMeta"]["limits"][L] = limitJSON;
        }
        noiseJSON["overwriteData"] = {};
        noiseJSON["overwriteData"]["fillNoiseData"] = fillNoiseData;
        noiseJSON["overwriteData"]["fillType"] = fillType;
        noiseJSON["overwriteData"]["fillEmptySlots"] = fillEmptySlots;
        noiseJSON["overwriteData"]["whitelist"] = whitelist;
        noiseJSON["overwriteData"]["blacklist"] = blacklist;
        noiseJSON["overwriteData"]["replaceColliding"] = replaceColliding;
        noiseJSON["overwriteData"]["forceReplaceAll"] = forceReplaceAll;
        noiseJSON["overwriteData"]["ignoreColliding"] = ignoreColliding;
        noiseJSON["overwriteData"]["placeOnly"] = placeOnly;

        return noiseJSON;
    }
};
