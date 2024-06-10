package Main;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

public class CharacterDialogue {

    // Method to save character dialogue to a text file using Key-Value pairs
    public static void saveDialogueToFile() {
        Map<String, String> dialogue = new HashMap<>();
        dialogue.put("Line1", "Hello, I'm PlayerOne.");
        dialogue.put("Line2", "Nice to meet you!");
        dialogue.put("Line3", "What a beautiful day.");
        dialogue.put("Line4", "I'm ready for an adventure.");
        dialogue.put("Line5", "Let's explore!");

        try (PrintWriter writer = new PrintWriter("dialogue.txt")) {
            for (Map.Entry<String, String> entry : dialogue.entrySet()) {
                writer.println(entry.getKey() + "=" + entry.getValue());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to load dialogue from a text file into a HashMap
    public static Map<String, String> loadDialogueFromFile() {
        Map<String, String> dialogueMap = new HashMap<>();
        try (BufferedReader reader = new BufferedReader(new FileReader("dialogue.txt"))) {
            String line;
            while ((line = reader.readLine()) != null) {
                StringTokenizer tokenizer = new StringTokenizer(line, "=");
                String key = tokenizer.nextToken();
                String value = tokenizer.nextToken();
                dialogueMap.put(key, value);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return dialogueMap;
    }

    public static void main(String[] args) {
        saveDialogueToFile(); // Save character dialogue to a file

        // Load dialogue from the file into a HashMap
        Map<String, String> characterDialogue = loadDialogueFromFile();

        // Retrieve a single line of text from the HashMap using the "get" method
        String lineToDisplay = characterDialogue.get("Line3"); // Example: Retrieving Line3

        // Display the retrieved line of text using the Gaming API drawString method
        // Replace the following line with your actual Gaming API drawString method
        System.out.println("Displaying line at location 100x, 250y: " + lineToDisplay);
    }
}