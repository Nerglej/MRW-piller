/** @type {import('./$types').PageLoad} */
export async function load({ fetch }) {
    let data = await fetch('/')

    let jsonData = await data.json()
    console.log(jsonData)

    return {
        entries: jsonData
    }
}