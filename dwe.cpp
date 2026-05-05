using System;
using System.Collections.Generic;

class Storage<T>
{
    private List<T> items = new List<T>();

    public void Add(T item)
    {
        items.Add(item);
    }

    public List<T> GetAll()
    {
        return items;
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("Работа с типом int:");
        Storage<int> intStorage = new Storage<int>();
        intStorage.Add(10);
        intStorage.Add(20);
        intStorage.Add(30);

        List<int> intItems = intStorage.GetAll();
        foreach (int item in intItems)
        {
            Console.WriteLine(item);
        }

        Console.WriteLine();

        Console.WriteLine("Работа с типом string:");
        Storage<string> stringStorage = new Storage<string>();
        stringStorage.Add("Привет");
        stringStorage.Add("Мир");
        stringStorage.Add("C#");

        List<string> stringItems = stringStorage.GetAll();
        foreach (string item in stringItems)
        {
            Console.WriteLine(item);
        }
    }
}


// Задание 2 

using System;
using System.IO;

class FileHandler
{
    static void Main()
    {
        string filePath = "data.txt";

        try
        {
            Console.WriteLine("Содержимое файла:");
            string content = File.ReadAllText(filePath);
            Console.WriteLine(content);
        }
        catch (FileNotFoundException)
        {
            Console.WriteLine($"Ошибка: Файл '{filePath}' не найден.");
            return;
        }
        catch (IOException ex)
        {
            Console.WriteLine($"Ошибка чтения файла: {ex.Message}");
            return;
        }

        try
        {
            string newLine = "Новая строка";
            File.AppendAllText(filePath, newLine + Environment.NewLine);
            Console.WriteLine($"\nСтрока '{newLine}' успешно добавлена в файл.");
        }
        catch (IOException ex)
        {
            Console.WriteLine($"Ошибка записи в файл: {ex.Message}");
        }
    }
}
