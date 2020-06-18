export interface DefaultData {
    success: boolean
}

export interface DefaultResponse {
    data: DefaultData
}

export interface GetCurtainStatusResponse {
    data: DefaultData & {
        status: "closed" | "open" | "closing" | "opening" | "idle"
    }
}
