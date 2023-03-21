import { json } from '@sveltejs/kit';
import { db } from '$lib/db';

export async function GET() {
    // Read from the database.
    await db.read()

    // Make a default entry if none is present.
    db.data ||= {
        entries: []
    }

    // Return the database contents.
    return json(db.data.entries)
}

/// Recieves a request, so it can be logged in the database. 
/// It should look like this:
/// {
///     "id": "id for the person",
///     "productId": "id for the product"
///     "data": {
///         "pulse": 80
///      }
/// }

/** @type {import('./$types').RequestHandler} */
export async function POST({ request }) {
    let jsonReq = await request.json()

    // Check if request is correct.
    if (!jsonReq.hasOwnProperty('id') || !jsonReq.hasOwnProperty('serial')) {
        return new Response(JSON.stringify({ error: "Properties are missing: 'id' or 'productId'" }), { status: 400 });
    }

    // Add a date to the entry.
    jsonReq.date = new Date();

    // Load db
    await db.read()

    // Default value
    db.data ||= {
        entries: []
    }

    // Push the entry to the entries property in the db.
    db.data.entries.push(jsonReq)

    // Write the db so it actually saves.
    await db.write()

    return new Response(JSON.stringify({ success: true, info: "Successfully saved to db" }), { status: 200 });
}