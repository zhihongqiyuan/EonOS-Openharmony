# Cookbook Convertor

Cookbook convertor is a tool that generates rule descriptions for TS Linter from Cookbook receipts. Convertor accepts RST file that contains the sources for Cookbook receipts.

## Prerequisits

NodeJS 18 or later.

## Building

Run the following command to build the cookbook convertor project:

```bash
npm install
```

or:

```bash
npm run build
```

## Running

Run the following command from the same directory:

```bash
node build/cookbook_convertor.js [rst_file]
```
