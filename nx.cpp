//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright © 2013 Peter Atashian                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "nx.hpp"
#include "file.hpp"
#include "node.hpp"
#include <io.h>
#include <fstream>
#include <unordered_map>
#include "iostream"

#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

namespace nl
{
    namespace nx
    {
        std::vector<std::unique_ptr<file>> files;
        std::unordered_map<std::string, node> nodes;

        bool exists(std::string name)
        {
            return std::ifstream(name).is_open();
        }

        node add_file(std::string name)
        {
            if (!exists(name))
                return {};

            files.emplace_back(new file(name));

            return *files.back();
        }

        node base, character, effect, etc, item, map, mob, morph, npc, quest, reactor, skill, sound, string, tamingmob, ui, mobSkill;

        void getFiles(std::string path, std::string oriPath, std::string parentNodeName) {
            intptr_t   hFile = 0;
            struct _finddata_t fileinfo;
            std::string p;
            if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
                do {
                    if ((fileinfo.attrib &  _A_SUBDIR))//是否為資料夾
                    {
                        if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                            std::string name = fileinfo.name;
                            auto node = parentNodeName + "/" + name;
                            getFiles(p.assign(path).append("\\").append(fileinfo.name), oriPath + name + "\\", node);
                        }
                    } else {
                        std::string fileName = fileinfo.name;
                        std::string name = fileName;
                        name.erase(name.end() - 3, name.end());
                        auto file = add_file(oriPath + fileName);
                        nodes[parentNodeName + "/" + name + ".img"] = file;
                    }
                } while (_findnext(hFile, &fileinfo) == 0);
                _findclose(hFile);
            }
        }

        void load_all()
        {

            if (exists("Base.nx"))
            {
                base = add_file("Base.nx");
                character = add_file("Character.nx");
                getFiles("./Data/Character", ".\\Data\\Character\\", "Character");
                effect = add_file("Effect.nx");
                getFiles("./Data/Effect", ".\\Data\\Effect\\", "Effect");
                etc = add_file("Etc.nx");
                getFiles("./Data/Etc", ".\\Data\\Etc\\", "Etc");
                item = add_file("Item.nx");
                getFiles("./Data/Item", ".\\Data\\Item\\", "Item");
                map = add_file("Map.nx");
                getFiles("./Data/Map", ".\\Data\\Map\\", "Map");
                mob = add_file("Mob.nx");
                getFiles("./Data/Mob", ".\\Data\\Mob\\", "Mob");
                morph = add_file("Morph.nx");
                getFiles("./Data/Morph", ".\\Data\\Morph\\", "Morph");
                npc = add_file("Npc.nx");
                getFiles("./Data/Npc", ".\\Data\\Npc\\", "Npc");
                quest = add_file("Quest.nx");
                getFiles("./Data/Quest", ".\\Data\\Quest\\", "Quest");
                reactor = add_file("Reactor.nx");
                getFiles("./Data/Reactor", ".\\Data\\Reactor\\", "Reactor");
                skill = add_file("Skill.nx");
                getFiles("./Data/Skill", ".\\Data\\Skill\\", "Skill");
                mobSkill = add_file("MobSkill.nx");
                sound = add_file("Sound.nx");
                getFiles("./Data/Sound", ".\\Data\\Sound\\", "Sound");
                string = add_file("String.nx");
                getFiles("./Data/String", ".\\Data\\String\\", "String");
                tamingmob = add_file("TamingMob.nx");
                getFiles("./Data/TamingMob", ".\\Data\\TamingMob\\", "TamingMob");
                ui = add_file("UI.nx");
                getFiles("./Data/UI", ".\\Data\\UI\\", "UI");
            }
            else if (exists("Data.nx"))
            {
                base = add_file("Data.nx");
                character = base["Character"];
                effect = base["Effect"];
                etc = base["Etc"];
                item = base["Item"];
                map = base["Map"];
                mob = base["Mob"];
                morph = base["Morph"];
                npc = base["Npc"];
                quest = base["Quest"];
                reactor = base["Reactor"];
                skill = base["Skill"];
                mobSkill = base["MobSkill"];
                sound = base["Sound"];
                string = base["String"];
                tamingmob = base["TamingMob"];
                ui = base["UI"];
            }
            else
            {
                throw std::runtime_error("Failed to locate nx files.");
            }
        }
    }
}
