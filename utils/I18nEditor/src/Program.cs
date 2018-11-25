using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace I18nEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // List<String> patterns = DataCompressor.FindPatterns("A long time ago in a galaxy far, far, away...", "Hello, World!", "Lorem Ipsum");
            // List<String> patterns = DataCompressor.FindPatterns("item.bow.name", 
            //                                                     "item.bat.name", 
            //                                                     "item.crossbow.name",
            //                                                     "item.shotgun.name",
            //                                                     "item.30-06-rifle.name",
            //                                                     "object.floor.name",
            //                                                     "object.wall.name",
            //                                                     "object.window.name",
            //                                                     "object.windowed_wall.name");
            // // List<String> patterns = DataCompressor.FindPatterns("Hello");
            // for(int i = 0; i < patterns.Count; ++i)
            // {
            //     Console.WriteLine("Pattern {0}: {1}", i, patterns[i]);
            // }

            string[] unlocalizedStrings = {
                "item.bow.name",
                "item.bat.name",
                "item.crossbow.name",
                "item.shotgun.name",
                "item.30-06-rifle.name",
                "object.floor.name",
                "object.wall.name",
                "object.window.name",
                "object.windowed_wall.name",
                "message.invalid_setting.name",
                "message.player_out_of_bounds.name",
                "message.error_reading_save.name",
                "message.debug.player_pos.name",
                "message.debug.fps.name",
                "message.debug.shots_per_second.name",
                "message.debug.player_shots_per_second.name",
                "message.debug.npc_shots_per_second.name",
                "message.debug.companion_shots_per_second.name",
                "message.debug.enemy_shots_per_second.name",
                "object.campfire.name",
                "object.cooking_station.name",
                "object.chemistry_set.name",
                "object.light.name",
                "object.lamp.name",
                "object.chair.name",
                "object.bed.name",
                "object.couch.name",
                "object.radio.name",
                "object.door.name",
                "item.sword.name",
                "item.bayonet.name",
                "item.sniper_rifle.name",
                "perk.strength.name",
                "perk.perception.name",
                "perk.speech.name",
                "perk.hacking.name",
                "perk.lock_picking.name",
                "perk.small_guns.name",
                "perk.big_guns.name",
                "perk.laser_weapons.name",
            };

            int byteCount = 0;

            foreach(string s in unlocalizedStrings)
            {
                byteCount += s.Length;
            }

            Stopwatch sw = new Stopwatch();
            sw.Start();
            byte[] bytes = DataCompressor.Compress(unlocalizedStrings);
            sw.Stop();

            long compressTime = sw.ElapsedMilliseconds;
            sw = new Stopwatch();
            

            // Console.WriteLine("Compressed Bytes {0}", BitConverter.ToString(bytes));
            // Console.WriteLine("Compressed Bytes as String {0}", Encoding.UTF8.GetString(bytes));

            sw.Start();
            string[] strings = DataCompressor.Decompress(bytes);
            sw.Stop();

            long decompressTime = sw.ElapsedMilliseconds;

            sw = new Stopwatch();

            sw.Start();
            List<string> patterns = DataCompressor.FindPatterns(32, 16, 6, 4, unlocalizedStrings);
            sw.Stop();

            for(int i = 0; i < strings.Length; ++i)
            {
                Console.WriteLine("Locale {0}: {1}", i, strings[i]);
            }

            Console.WriteLine("Uncompressed Size: {0}\nCompressed Size: {1}\nRatio: {2}", byteCount, bytes.Length, (float) bytes.Length / (float) byteCount);
            Console.WriteLine("Compression Time: {0}\nDecompression Time: {1}\nPattern Identification Time: {2}", compressTime, decompressTime, sw.ElapsedMilliseconds);

            File.WriteAllBytes("compressedLocale.blang", bytes);

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new LocalizationEditorForm());
        }
    }
}
