using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using ManualMapInjection.Injection;

namespace ManualMapInjection
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter process name: ");
            var name = Console.ReadLine();
            var target = Process.GetProcessesByName(name).FirstOrDefault();

            if (target == null)
            {
                return;
            }

            Console.Write("Enter dll: ");
            var path = Console.ReadLine();

            if (!File.Exists(path))
            {
                return;
            }

            var file = File.ReadAllBytes(path);
            var injector = new ManualMapInjector(target) {AsyncInjection = true};

            Console.WriteLine($"hmodule = 0x{injector.Inject(file).ToInt64():x8}");
            Console.ReadKey();
        }
    }
}
