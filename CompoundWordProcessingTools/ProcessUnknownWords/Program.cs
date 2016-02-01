using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProcessUnknownWords
{
    class Program
    {
        /// <summary>
        /// Processes unknown words in Moses input files with a compound splitter.
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            string oovFile = null;
            string inFile = null;
            string outFile = null;
            string outFileProcessed = null;
            string splitCompoundsPath = null;
            string splitCompoundsArgs = null;
            bool splitDashWords = false;
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-u" && args.Length > i + 1)
                {
                    oovFile = args[i + 1];
                }
                if (args[i] == "-i" && args.Length > i + 1)
                {
                    inFile = args[i + 1];
                }
                if (args[i] == "-o" && args.Length > i + 1)
                {
                    outFile = args[i + 1];
                    outFileProcessed = args[i + 1] + ".proc";
                }
                else if (args[i] == "-scp" && args.Length > i + 1)
                {
                    splitCompoundsPath = args[i + 1];
                }
                else if (args[i] == "-sca" && args.Length > i + 1)
                {
                    splitCompoundsArgs = args[i + 1];
                }
                else if (args[i] == "-s")
                {
                    splitDashWords = true;
                }
            }            

            List<Dictionary<string, string>> processedOOVs = ProcessOOVs(oovFile, outFile + ".tmp", splitCompoundsPath, splitCompoundsArgs, splitDashWords);
            StreamReader sr = new StreamReader(inFile, Encoding.UTF8);
            StreamWriter sw = new StreamWriter(outFile, false, new UTF8Encoding(false));
            StreamWriter swProc = new StreamWriter(outFileProcessed, false, new UTF8Encoding(false));
            sw.NewLine = "\n";
            swProc.NewLine = "\n";
            char[] sep = {' '};
            int idx = 0;
            int sentencesProcessedTwice = 0;
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine().Trim();
                string[] arr = line.Split(sep, StringSplitOptions.RemoveEmptyEntries);
                bool found = false;
                for (int i = 0;i<arr.Length;i++)
                {
                    if (i > 0) sw.Write(" ");
                    string toWrite = arr[i];
                    if (processedOOVs[idx].ContainsKey(arr[i]))
                    {
                        toWrite = processedOOVs[idx][arr[i]];
                        found = true;
                    }
                    sw.Write(toWrite);
                }
                if (found)
                {
                    sentencesProcessedTwice++;
                    swProc.WriteLine("1");
                }
                else
                {
                    swProc.WriteLine();
                }
                sw.WriteLine();
                idx++;
            }
            swProc.Close();
            sr.Close();
            sw.Close();
            Console.Error.WriteLine("sentencesProcessedTwice: " + sentencesProcessedTwice.ToString());
        }

        private static List<Dictionary<string, string>> ProcessOOVs(string oovFile, string tempFile, string splitCompoundsPath, string args, bool splitDashWords = false)
        {
            StreamReader sr = new StreamReader(oovFile, Encoding.UTF8);
            char[] sep = { ' ' };
            List<Dictionary<string, string>> res = new List<Dictionary<string, string>>();
            bool addGammas = splitCompoundsPath.Contains("-a");
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine().Trim();
                res.Add(new Dictionary<string, string>());
                int idx = res.Count - 1;
                if (!string.IsNullOrWhiteSpace(line))
                {
                    string[] arr = line.Split(sep, StringSplitOptions.RemoveEmptyEntries);
                    for (int i = 0; i < arr.Length; i++)
                    {
                        Process queryProcess = new Process();
                        queryProcess.StartInfo.UseShellExecute = false;
                        queryProcess.StartInfo.RedirectStandardInput = true;
                        queryProcess.StartInfo.RedirectStandardOutput = true;
                        queryProcess.StartInfo.FileName = splitCompoundsPath;
                        queryProcess.StartInfo.Arguments = args;
                        queryProcess.Start();
                        queryProcess.StandardInput.AutoFlush = true;

                        queryProcess.StandardInput.WriteLine(arr[i]);
                        queryProcess.StandardInput.Close();
                        string output = queryProcess.StandardOutput.ReadToEnd();
                        queryProcess.WaitForExit();
                        queryProcess.Dispose();
                        queryProcess.Close();
                        if (output != null && splitDashWords)
                        {
                            StringBuilder sb = new StringBuilder(output.Length + 8);
                            for (int j = 0; j < output.Length; j++)
                            {
                                if (output[j] == '-' && j > 0 && Char.IsLetterOrDigit(output[j - 1]))
                                {
                                    if (addGammas)
                                        sb.Append("γ γ");
                                    else
                                        sb.Append(" ");
                                }
                                sb.Append(output[j]);
                                if (output[j] == '-' && j + 1 < output.Length && Char.IsLetterOrDigit(output[j + 1]))
                                {
                                    if (addGammas)
                                        sb.Append("γ γ");
                                    else
                                        sb.Append(" ");
                                }
                            }
                            output = sb.ToString();
                        }
                        if (output != null && output.Trim() != arr[i] && output.Contains(" "))
                        {
                            if (!res[idx].ContainsKey(arr[i])) res[idx].Add(arr[i], output.Trim());
                        }
                    }
                }
            }
            sr.Close();
            return res;
        }
    }
}
