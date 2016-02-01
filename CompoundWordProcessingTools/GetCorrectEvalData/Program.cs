using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace GetCorrectEvalData
{
    class Program
    {
        static void Main(string[] args)
        {
            string unkFile = null;
            string firstOutputFile = null;
            string secondOutputFile = null;
            string newOutputFile = null;
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-p" && args.Length > i + 1)
                {
                    unkFile = args[i + 1];
                }
                if (args[i] == "-i1" && args.Length > i + 1)
                {
                    firstOutputFile = args[i + 1];
                }
                if (args[i] == "-i2" && args.Length > i + 1)
                {
                    secondOutputFile = args[i + 1];
                }
                else if (args[i] == "-o" && args.Length > i + 1)
                {
                    newOutputFile = args[i + 1];
                }
            }

            StreamReader firstSr = new StreamReader(firstOutputFile, Encoding.UTF8);
            StreamReader unkSr = new StreamReader(unkFile, Encoding.UTF8);
            StreamReader secondSr = new StreamReader(secondOutputFile, Encoding.UTF8);
            StreamWriter sw = new StreamWriter(newOutputFile, false, new UTF8Encoding(false));
            sw.NewLine = "\n";

            while(!firstSr.EndOfStream)
            {
                string firstLine = firstSr.ReadLine().Trim();
                string secondLine = secondSr.ReadLine().Trim();
                string unkLine = unkSr.ReadLine().Trim();
                if (string.IsNullOrWhiteSpace(unkLine))
                {
                    sw.WriteLine(firstLine);
                }
                else
                {
                    sw.WriteLine(secondLine);
                }
            }

            firstSr.Close();
            unkSr.Close();
            secondSr.Close();
            sw.Close();
        }
    }
}
