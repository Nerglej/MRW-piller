/** @type {import('./$types').PageLoad} */
export async function load({ fetch }) {
    let data = await fetch('/')

    let jsonData = await data.json()

    return {
        entries: jsonData
    }
}