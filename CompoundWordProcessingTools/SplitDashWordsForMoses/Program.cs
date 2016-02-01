using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SplitDashWordsForMoses
{
    public class Program
    {
        static void Main(string[] args)
        {
            string inFile = null;
            string outFile = null;
            bool addGammas = false;

            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-o" && args.Length > i + 1)
                {
                    outFile = args[i + 1];
                }
                else if (args[i] == "-i" && args.Length > i + 1)
                {
                    inFile = args[i + 1];
                }
                else if (args[i] == "-a")
                {
                    addGammas = true;
                }
            }            

            TextReader reader;
            TextWriter writer;

            if (!string.IsNullOrWhiteSpace(inFile))
            {
                reader = new StreamReader(inFile, Encoding.UTF8);
            }
            else
            {
                reader = Console.In;
            }
            if (!string.IsNullOrWhiteSpace(outFile))
            {
                writer = new StreamWriter(outFile, false, new UTF8Encoding(false));
                writer.NewLine = "\n";
            }
            else
            {
                writer = Console.Out;
                writer.NewLine = "\n";
            }
            DateTime started = DateTime.Now;
            int lines = 0;
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                lines++;
                line = line.Trim();
                StringBuilder sb = new StringBuilder(line.Length + 8);
                for (int i = 0; i < line.Length; i++)
                {
                    if (line[i] == '-' && i > 0 && Char.IsLetterOrDigit(line[i - 1]))
                    {
                        if (addGammas)
                            sb.Append("γ γ");
                        else
                            sb.Append(" ");
                    }
                    sb.Append(line[i]);
                    if (line[i] == '-' && i + 1 < line.Length && Char.IsLetterOrDigit(line[i + 1]))
                    {
                        if (addGammas)
                            sb.Append("γ γ");
                        else
                            sb.Append(" ");
                    }
                }
                writer.WriteLine(sb.ToString());
            }
            if (!string.IsNullOrWhiteSpace(inFile))
            {
                reader.Close();
            }
            if (!string.IsNullOrWhiteSpace(outFile))
            {
                writer.Close();
                Console.Error.WriteLine("Lines {0} time {1} {2} lines/sec", lines, DateTime.Now - started, lines / (DateTime.Now - started).TotalSeconds);
            }
        }
    }
}
