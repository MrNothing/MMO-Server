using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpClient
{
    public class JsonParser
    {
        public static string ListToJson(List<string> data)
        {
            string result = "";

            return result;

        }
        public static string HashtableToJson(Hashtable data)
        {
            string result="";

            return result;
        }

        public static string DictionnaryToJson<T1, T2>(Dictionary<T1, T2> data)
        {
            string result = "";

            return result;
        }

        public static Hashtable JsonToHashTable(string json)
        {
            Hashtable result = new Hashtable();

            int level = 0;

            bool isString = false;
            bool isKey = false;

            string lastKey = "";
            string buffer = "";

            int lastLvl1Index = 0;

            for (int i = 0; i < json.Length; i++)
            {
                if (json[i] == '{' && !isString)
                {
                    level++;

                    if (level == 1)
                        isKey = true;

                    if (level == 2)
                        lastLvl1Index = i;
                }
                else if (json[i] == ':' && !isString)
                {
                    if (level == 1)
                    {
                        lastKey = buffer;

                        isKey = false;

                        //flush the buffer
                        buffer = "";
                    }

                }
                else if (json[i] == ',' && !isString)
                {
                    if (level == 1)
                    {
                        result.Add(lastKey, buffer);

                        isKey = true;

                        buffer = "";
                    }

                }
                else if (json[i] == '}' && !isString)
                {
                    level--;

                    //we were in deeper levels, this was an object...
                    if (level == 1)
                    {
                        result.Add(lastKey, JsonToHashTable(json.Substring(lastLvl1Index, i - lastLvl1Index + 1)));
                        buffer = "";
                    }

                    if (level == 0)
                    {
                        if (buffer.Length > 0)
                            result.Add(lastKey, buffer);
                        buffer = "";
                    }
                }
                else if (json[i] == '"')
                {
                    isString = !isString;
                }
                else
                {
                    if ((isKey && isString) || (!isKey && (isString || json[i] != ' ')))
                        buffer += json[i];
                }
            }

            return result;
        }

        public static Dictionary<string, Object> JsonToDictionary(string json)
        {
            Dictionary<string, Object> result = new Dictionary<string, Object>();

            int level = 0;

            bool isString = false;
            bool isKey = false;

            string lastKey = "";
            string buffer = "";

            int lastLvl1Index = 0;

            for (int i = 0; i < json.Length; i++)
            {
                if (json[i] == '{' && !isString)
                {
                    level++;

                    if (level == 1)
                        isKey = true;

                    if (level == 2)
                        lastLvl1Index = i;
                }
                else if (json[i] == ':' && !isString)
                {
                    if (level == 1)
                    {
                        isKey = false;

                        lastKey = buffer;

                        //flush the buffer
                        buffer = "";
                    }

                }
                else if (json[i] == ',' && !isString)
                {
                    if (level == 1)
                    {
                        isKey = true;

                        result.Add(lastKey, buffer);
                        buffer = "";
                    }

                }
                else if (json[i] == '}' && !isString)
                {
                    level--;

                    //we were in deeper levels, this was an object...
                    if (level == 1)
                    {
                        result.Add(lastKey, JsonToDictionary(json.Substring(lastLvl1Index, i - lastLvl1Index + 1)));
                        buffer = "";
                    }

                    if (level == 0)
                    {
                        if (buffer.Length > 0)
                            result.Add(lastKey, buffer);
                        buffer = "";
                    }
                }
                else if (json[i] == '"')
                {
                    isString = !isString;
                }
                else
                {
                    if ((isKey && isString) || (!isKey && (isString || json[i]!= ' ')))
                        buffer += json[i];
                }
            }

            return result;
        }
    }
}
