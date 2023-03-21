import { join, dirname } from 'node:path'
import { fileURLToPath } from 'node:url'

import { Low, JSONFile } from "lowdb";

const __dirname = dirname(fileURLToPath(import.meta.url));
const file = join(__dirname, 'db.json')

const adapter = new JSONFile(file)
const db = new Low(adapter)

export {
    db
}